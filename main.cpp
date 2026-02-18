#include <set>
#include <gmsh.h>
#include <cmath>
#include <vector>

struct Point {
  double x, y, z;
  int tag;
};

void buildCircle(double r, double l) {
  Point Q = {l, 0.0, 0.0, 1};
  gmsh::model::occ::addCircle(Q.x, Q.y, Q.z, r, 1);
}

void revolveShape(int curveTag) {
  int loopTag = gmsh::model::occ::addCurveLoop({curveTag});
  int surfTag = gmsh::model::occ::addPlaneSurface({loopTag});

  std::vector<std::pair<int,int>> out;
  gmsh::model::occ::revolve({{2, surfTag}}, 0, 0, 0,  0, 0, 1,  1*M_PI - 0.1,  out);
  
}

int main(int argc, char **argv) {
  gmsh::initialize();
  gmsh::model::add("Torus");

  double r = 1.0;
  double l = 4.0;

  buildCircle(r, l);
  revolveShape(1);

  gmsh::model::occ::synchronize();
  gmsh::model::mesh::generate(3);

  std::set<std::string> args(argv, argv + argc);
  if(!args.count("-nopopup")) gmsh::fltk::run();

  gmsh::finalize();

  return 0;
}