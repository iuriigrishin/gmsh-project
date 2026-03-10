#include <set>
#include <gmsh.h>
#include <cmath>
#include <vector>

struct TorusParams {
  double R;
  double r;
};

int CreateTorusProfile(const TorusParams &params) {
  double R = params.R;
  double r = params.r;

  // xz plane
  int p1 = gmsh::model::occ::addPoint(R + r, 0.0,  0.0);
  int p2 = gmsh::model::occ::addPoint(R,     0.0,  r  );
  int p3 = gmsh::model::occ::addPoint(R - r, 0.0,  0.0);
  int p4 = gmsh::model::occ::addPoint(R,     0.0, -r  );

  int pc = gmsh::model::occ::addPoint(R, 0.0, 0.0);

  int a1 = gmsh::model::occ::addCircleArc(p1, pc, p2);
  int a2 = gmsh::model::occ::addCircleArc(p2, pc, p3);
  int a3 = gmsh::model::occ::addCircleArc(p3, pc, p4);
  int a4 = gmsh::model::occ::addCircleArc(p4, pc, p1);

  int loopTag = gmsh::model::occ::addCurveLoop({a1, a2, a3, a4});
  return loopTag;
}

std::vector<std::pair<int,int>> CreateTorusVolume(int profileLoopTag) {
  int surfTag = gmsh::model::occ::addPlaneSurface({profileLoopTag});
  std::vector<std::pair<int,int>> out;
  gmsh::model::occ::revolve(
    {{2, surfTag}},
    0.0, 0.0, 0.0,
    0.0, 0.0, 1.0,
    2.0 * M_PI,
    out
  );
  return out;
}

std::vector<std::pair<int,int>> CreateTorus(const TorusParams &params) {
  int profileCurveTag = CreateTorusProfile(params);

  return CreateTorusVolume(profileCurveTag);
}


void BuildMesh(double h) {
  gmsh::option::setNumber("Mesh.CharacteristicLengthMin", h);
  gmsh::option::setNumber("Mesh.CharacteristicLengthMax", h);

  gmsh::model::occ::synchronize();
  gmsh::model::mesh::generate(3);
}

int main(int argc, char **argv) {
  gmsh::initialize();
  gmsh::model::add("Torus");

  TorusParams torus{4.0, 1.0};

  auto created = CreateTorus(torus); 
  
  gmsh::model::occ::synchronize();
  BuildMesh(0.6);


  std::set<std::string> args(argv, argv + argc);
  if(!args.count("-nopopup")) gmsh::fltk::run();

  gmsh::finalize();

  return 0;
}