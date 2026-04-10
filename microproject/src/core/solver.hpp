#pragma once

#include "core/grid.hpp"
#include <vector>

class Solver {
 public:
  explicit Solver(const Grid& grid);
 
  void step(Grid& grid, double dt);
 
 private:
  std::vector<double> H_new_;
};
