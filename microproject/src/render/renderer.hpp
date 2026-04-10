#pragma once

#include "core/grid.hpp"

class Renderer {
public:
  Renderer(int window_size);
  void draw(const Grid& grid) const;

private:
  int window_size_;
  double T_min_ = -20.0;
  double T_max_ =  30.0;
};
