#include "scenes/stefan_1d.hpp"

void StefanValidation::init(Grid& grid) {
  int n = grid.size();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (j == 0) {
        grid.H(i, j) = grid.H_from_T_liquid(T_hot);
      } else {
        grid.H(i, j) = grid.H_from_T_solid(-0.01);
      }
    }
  }
}
