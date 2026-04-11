#pragma once

#include "core/grid.hpp"
#include "core/boundary.hpp"
#include <vector>

class Solver {
 private:
  std::vector<double> H_new_;
  BoundaryType bc_;
  double time_  = 0.0;
  int steps_ = 0;

 public:
  explicit Solver(const Grid& grid, BoundaryType bc = BoundaryType::Fixed);
 
  void step(Grid& grid, double dt);
  double time() const {
    return time_;
  }
  int steps() const {
    return steps_;
  }
};

