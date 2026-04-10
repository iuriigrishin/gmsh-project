#include "core/solver.hpp"

Solver::Solver(const Grid& grid): H_new_(grid.size() * grid.size(), 0.0)
{}

void Solver::step(Grid& grid, double dt) {
  int n = grid.size();
  double dx = grid.dx();
  const sim::SimMaterial& mat = grid.material();

  // 1. Для каждой внутренней ячейки (i от 1 до n-2, j от 1 до n-2):
  //    - взять T центра и четырёх соседей
  //    - посчитать лапласиан: (T_left + T_right + T_up + T_down - 4*T_center) / (dx*dx)
  //    - взять теплопроводность k в этой ячейке
  //    - обновить: H_new = H_old + dt * k * laplacian
  //
  // 2. Граничные ячейки (i==0, i==n-1, j==0, j==n-1):
  //    - просто скопировать H_old в H_new (фиксированная температура)
  //
  // 3. Записать H_new обратно в grid

  // --- твой код здесь ---
}