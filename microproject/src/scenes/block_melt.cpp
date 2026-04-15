#include "scenes/block_melt.hpp"

void BlockMelt::init(Grid& grid) {
  int n = grid.size();

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (j < n / 2) {
        grid.H(i, j) = grid.H_from_T_solid(-10.0);
      } else {
        grid.H(i, j) = grid.H_from_T_liquid(40.0);
      }
    }
  }
}
