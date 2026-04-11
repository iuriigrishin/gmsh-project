#include "core/solver.hpp"
#include "core/boundary.hpp"
Solver::Solver(const Grid& grid, BoundaryType bc): H_new_(grid.size() * grid.size(), 0.0), bc_(bc)
{}

void Solver::step(Grid& grid, double dt) {
  int n = grid.size();
  double dx = grid.dx();

  for (int i = 0; i < n; ++i) {
    H_new_[i * n + 0] = grid.H(i, 0);
    H_new_[i * n + (n - 1)] = grid.H(i, n - 1);
  }
  for (int j = 0; j < n; ++j) {
    H_new_[0 * n + j] = grid.H(0, j);
    H_new_[(n - 1) * n + j] = grid.H(n - 1, j);
  }

  for (int i = 1; i < n - 1; ++i) {
    for (int j = 1; j < n - 1; ++j) {
      double T_center = grid.T(i, j);
      double T_left   = grid.T(i, j - 1);
      double T_right  = grid.T(i, j + 1);
      double T_up     = grid.T(i - 1, j);
      double T_down   = grid.T(i + 1, j);
      double laplacian = (T_left + T_right + T_up + T_down - 4.0 * T_center) / (dx * dx);
 
      double k = grid.conductivity(i, j);
 
      H_new_[i * n + j] = grid.H(i, j) + dt * k * laplacian;
    }
  }

  if (bc_ == BoundaryType::Fixed) { // copy
    for (int i = 0; i < n; ++i) {
      H_new_[i * n + 0] = grid.H(i, 0);
      H_new_[i * n + (n - 1)] = grid.H(i, n - 1);
    }
    for (int j = 0; j < n; ++j) {
      H_new_[0 * n + j] = grid.H(0, j);
      H_new_[(n - 1) * n + j] = grid.H(n - 1, j);
    }
  } else {
    for (int i = 0; i < n; ++i) {
      H_new_[i * n + 0] = H_new_[i * n + 1];
      H_new_[i * n + (n - 1)] = H_new_[i * n + (n - 2)];
    }
    for (int j = 0; j < n; ++j) {
      H_new_[0 * n + j] = H_new_[1 * n + j];
      H_new_[(n - 1) * n + j] = H_new_[(n - 2) * n + j];
    }
  }
  



  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      grid.H(i, j) = H_new_[i * n + j];
    }
  }

  time_ += dt;
  steps_++;
}
