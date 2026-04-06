#include <cmath>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkTetra.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridWriter.h>

#include <gmsh.h>

using namespace std;

class Mesh;

class Node {
    friend class Mesh;
protected:
    double x, y, z;
    double smth;
    double vx, vy, vz;

public:
    Node() : x(0), y(0), z(0), smth(0), vx(0), vy(0), vz(0) {}

    Node(double x, double y, double z)
        : x(x), y(y), z(z), smth(0), vx(0), vy(0), vz(0) {}

    Node(double x, double y, double z, double smth, double vx, double vy, double vz)
        : x(x), y(y), z(z), smth(smth), vx(vx), vy(vy), vz(vz) {}

    void move(double tau) {
    //     x += vx * tau;
    //     y += vy * tau;
    //   z += vz * tau;
    }
};

struct Cell {
    int gmsh_type;
    vector<size_t> node_ids;
};


class Mesh {
 protected:
    vector<Node> nodes_;
    vector<Cell> cells_;

    double c_, v0_, sigma_;
    Node hit_center_;

    double sim_step_     = 1e-2;
    double sim_duration_ = 1.0;

    double z_min_ = 0.0;
    double z_max_ = 85.0;

    const double REFLECTION          = 0.8; // отражение от границ
    const double ATTENUATION_EXP     = 0.005; // коэф затухания
    const double ATTENUATION_LINEAR  = 0.02; // 

 public:
    void SetHit(double c, double v0, double sigma, Node center) {
        c_          = c;
        v0_         = v0;
        sigma_      = sigma;
        hit_center_ = center;
    }

    void SetSim(double sim_step, double sim_duration) {
        sim_step_     = sim_step;
        sim_duration_ = sim_duration;
    }

    // Загрузка сетки из .msh — один в один как в sword/mesh.hpp
    void LoadFromMsh(const string &filename) {
        gmsh::open(filename);

        vector<size_t> nodeTags;
        vector<double> coords, parametricCoords;
        gmsh::model::mesh::getNodes(nodeTags, coords, parametricCoords);

        unordered_map<size_t, size_t> tag_to_index;
        nodes_.resize(nodeTags.size());

        z_min_ =  1e18;
        z_max_ = -1e18;

        for (size_t i = 0; i < nodeTags.size(); ++i) {
            nodes_[i].x = coords[3 * i + 0];
            nodes_[i].y = coords[3 * i + 1];
            nodes_[i].z = coords[3 * i + 2];
            tag_to_index[nodeTags[i]] = i;
            if (nodes_[i].z < z_min_) z_min_ = nodes_[i].z;
            if (nodes_[i].z > z_max_) z_max_ = nodes_[i].z;
        }

        vector<int> elementTypes;
        vector<vector<size_t>> elementTags, elementNodeTags;
        gmsh::model::mesh::getElements(elementTypes, elementTags, elementNodeTags);

        cells_.clear();
        for (size_t b = 0; b < elementTypes.size(); ++b) {
            int type = elementTypes[b];

            string name;
            int dim, order, numNodes, numPrimaryNodes;
            vector<double> localCoords;
            gmsh::model::mesh::getElementProperties(
                type, name, dim, order, numNodes, localCoords, numPrimaryNodes);

            if (dim != 3) continue;

            const auto &conn = elementNodeTags[b];
            size_t numElems = conn.size() / numNodes;

            for (size_t e = 0; e < numElems; ++e) {
                Cell cell;
                cell.gmsh_type = type;
                cell.node_ids.resize(numNodes);
                for (int j = 0; j < numNodes; ++j)
                    cell.node_ids[j] = tag_to_index.at(conn[e * numNodes + j]);
                cells_.push_back(std::move(cell));
            }
        }

        cout << "Загружено узлов: " << nodes_.size()
             << ", ячеек: " << cells_.size() << endl;
        cout << "Z диапазон: [" << z_min_ << ", " << z_max_ << "]" << endl;
    }

    void applyWave(const Node &source, Node &node, double t,
                   double c, double v0, double sigma) {
        double dx = node.x - source.x;
        double dy = node.y - source.y;
        double dz = node.z - source.z;
        double r = sqrt(dx*dx + dy*dy + dz*dz) + 1e-6;

        const double wave_offset  = r - c * t;
        const double exponent_abs = (wave_offset * wave_offset) / (sigma * sigma);
        const double exp_cutoff   = -log(0.001);

        if (exponent_abs > exp_cutoff) return;

        double nx = dx / r;
        double ny = dy / r;
        double nz = dz / r;
        double v = v0 * exp(-exponent_abs);

        double attenuation = exp(-ATTENUATION_EXP * r)
                           / (1.0 + ATTENUATION_LINEAR * r);


        node.vx   += nx * v * attenuation;
        node.vy   += ny * v * attenuation;
        node.vz   += nz * v * attenuation;
        node.smth += v * attenuation;
    }

    void PointHit(Node &node, double t) {
        node.vx = node.vy = node.vz = 0.0;
        node.smth = 0.0;


        applyWave(hit_center_, node, t, c_, v0_, sigma_);

        Node mirror_tip(hit_center_.x, hit_center_.y,
                        2.0 * z_max_ - hit_center_.z);
        applyWave(mirror_tip, node, t, c_, v0_ * REFLECTION, sigma_);

        Node mirror_base(hit_center_.x, hit_center_.y,
                         2.0 * z_min_ - hit_center_.z);
        applyWave(mirror_base, node, t, c_, v0_ * REFLECTION, sigma_);


        Node mirror_tip_base(hit_center_.x, hit_center_.y,
                             2.0 * z_min_ - mirror_tip.z);
        applyWave(mirror_tip_base, node, t, c_,
                  v0_ * REFLECTION * REFLECTION, sigma_);

        Node mirror_base_tip(hit_center_.x, hit_center_.y,
                             2.0 * z_max_ - mirror_base.z);
        applyWave(mirror_base_tip, node, t, c_,
                  v0_ * REFLECTION * REFLECTION, sigma_);
    }

    void doTimeStep(double time, double tau) {
        for (auto &node : nodes_) {
            PointHit(node, time);
            node.move(tau);
        }
    }

    void Snapshot(unsigned int snap_number) {
        vtkSmartPointer<vtkUnstructuredGrid> grid =
            vtkSmartPointer<vtkUnstructuredGrid>::New();

        vtkSmartPointer<vtkPoints> dumpPoints =
            vtkSmartPointer<vtkPoints>::New();

        auto smth = vtkSmartPointer<vtkDoubleArray>::New();
        smth->SetName("smth");

        auto vel = vtkSmartPointer<vtkDoubleArray>::New();
        vel->SetName("velocity");
        vel->SetNumberOfComponents(3);

        for (size_t i = 0; i < nodes_.size(); i++) {
            dumpPoints->InsertNextPoint(nodes_[i].x, nodes_[i].y, nodes_[i].z);

            double v[3] = {nodes_[i].vx, nodes_[i].vy, nodes_[i].vz};
            vel->InsertNextTuple(v);

            smth->InsertNextValue(nodes_[i].smth);
        }

        grid->SetPoints(dumpPoints);

        for (size_t i = 0; i < cells_.size(); i++) {
            vtkSmartPointer<vtkTetra> tetra = vtkSmartPointer<vtkTetra>::New();
            tetra->GetPointIds()->SetId(0, cells_[i].node_ids[0]);
            tetra->GetPointIds()->SetId(1, cells_[i].node_ids[1]);
            tetra->GetPointIds()->SetId(2, cells_[i].node_ids[2]);
            tetra->GetPointIds()->SetId(3, cells_[i].node_ids[3]);
            grid->InsertNextCell(tetra->GetCellType(), tetra->GetPointIds());
        }

        grid->GetPointData()->AddArray(vel);
        grid->GetPointData()->AddArray(smth);

        string fileName = "pencil-step-" + to_string(snap_number) + ".vtu";
        vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer =
            vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
        writer->SetFileName(fileName.c_str());
        writer->SetInputData(grid);
        writer->Write();
    }

    // Запуск симуляции — один в один из sword/mesh.hpp
    void RunSimulation() {
        int total = static_cast<int>(sim_duration_ / sim_step_);
        for (double time = 0.0; time < sim_duration_; time += sim_step_) {
            int step = static_cast<int>(time / sim_step_);
            doTimeStep(time, sim_step_);
            cout << "Step: " << step << " / " << total
                 << " (" << (time / sim_duration_ * 100.0) << "%)" << endl;
            Snapshot(step);
        }
    }
};

int main(int argc, char **argv) {

    //Строим объёмную сетку
    gmsh::initialize();
    gmsh::model::add("pencil");

    try {
        gmsh::merge("../pencil.stl");
    } catch (...) {
        gmsh::logger::write("Could not load STL mesh: bye!");
        gmsh::finalize();
        return 1;
    }

    double angle                    = 40;
    bool forceParametrizablePatches = false;
    bool includeBoundary            = true;
    double curveAngle               = 180;

    gmsh::model::mesh::classifySurfaces(angle * M_PI / 180., includeBoundary,
                                        forceParametrizablePatches,
                                        curveAngle * M_PI / 180.);
    gmsh::model::mesh::createGeometry();

    vector<pair<int, int>> s;
    gmsh::model::getEntities(s, 2);
    vector<int> sl;
    for (auto surf : s) sl.push_back(surf.second);

    int l = gmsh::model::geo::addSurfaceLoop(sl);
    gmsh::model::geo::addVolume({l});
    gmsh::model::geo::synchronize();

    const double h = 2.0;
    vector<pair<int, int>> points;
    gmsh::model::getEntities(points, 0);
    gmsh::model::mesh::setSize(points, h);

    gmsh::model::mesh::generate(3);
    gmsh::write("pencil.msh");

    set<string> args(argv, argv + argc);
    if (!args.count("-nopopup"))
        gmsh::fltk::run();

    // Загружаем сетку
    Mesh mesh;
    mesh.LoadFromMsh("pencil.msh");

    mesh.SetHit(15, 15.0, 3.0, Node(2, 0, 85));
    mesh.SetSim(0.05, 20.0);

    gmsh::finalize();

    mesh.RunSimulation();

    return 0;
}