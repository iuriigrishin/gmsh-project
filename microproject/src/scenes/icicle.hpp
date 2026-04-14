#pragma once
#include "scenes/scene.hpp"

class Icicle : public Scene {
public:
  void init(Grid& grid) override;
  std::string name() const override {
    return "Icicle";
  }
};