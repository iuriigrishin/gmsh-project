#include "render/renderer.hpp"
#include "render/colormap.hpp"
#include <raylib.h>

Renderer::Renderer(int window_size)
  : window_size_(window_size) {}

void Renderer::draw(const Grid& grid) const {
  int n = grid.size();
  float cell = static_cast<float>(window_size_) / n;

  for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        double T = grid.T(i, j);
        Color color = temperature_to_color(T, T_min_, T_max_);
        
        DrawRectangle(
          static_cast<int>(j * cell),
          static_cast<int>(i * cell),
          static_cast<int>(cell) + 1,
          static_cast<int>(cell) + 1,
          color
        );
    }
  }
}

