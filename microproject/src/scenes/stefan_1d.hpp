#pragma once

#include "scenes/scene.hpp"

class StefanValidation : public Scene {
 public:
  void init(Grid& grid) override;
  std::string name() const override {
    return "Stefan 1D Validation";
  }

  static constexpr double T_hot = 50.0;
};
