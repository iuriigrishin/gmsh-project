#include <set>
#include <gmsh.h>

namespace geo = gmsh::model::geo;

int main(int argc, char **argv)
{
  gmsh::initialize();
  gmsh::model::add("circle");

  double lc = 0.1;

  enum { C=1, P1, P2, P3, P4 };

  double r = 1.0;

  geo::addPoint(0,  0, 0, lc, C);

  geo::addPoint( r,  0, 0, lc, P1);
  geo::addPoint( 0,  r, 0, lc, P2);
  geo::addPoint(-r,  0, 0, lc, P3);
  geo::addPoint( 0, -r, 0, lc, P4);

  geo::addCircleArc(P1, C, P2, 1);
  geo::addCircleArc(P2, C, P3, 2);
  geo::addCircleArc(P3, C, P4, 3);
  geo::addCircleArc(P4, C, P1, 4);

  geo::addCurveLoop({1, 2, 3, 4}, 1);
  geo::addPlaneSurface({1}, 1);

  geo::synchronize();


  gmsh::model::mesh::generate(2);

  gmsh::write("circle.msh");


  std::set<std::string> args(argv, argv + argc);
  if(!args.count("-nopopup")) gmsh::fltk::run();

  gmsh::finalize();

  return 0;
}