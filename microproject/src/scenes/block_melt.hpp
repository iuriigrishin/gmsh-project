#pragma once

#include "scenes/scene.hpp"

class BlockMelt : public Scene {
public:
  void init(Grid& grid) override;
  std::string name() const override {
    return "Block Melt";
  }
};
