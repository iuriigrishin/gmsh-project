#include "scenes/icicle.hpp"

void Icicle::init(Grid& grid) {
  int n = grid.size();

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (i == 0) {
       grid.H(i, j) = grid.H_from_T_solid(-20.0);
      } else if (i == n - 1 || j == 0 || j == n - 1) {
        grid.H(i, j) = grid.H_from_T_liquid(5.0);
      } else {
        grid.H(i, j) = grid.H_from_T_liquid(2.0);
      }
    }
  }
}