#pragma once

#include "scenes/scene.hpp"

class Snowflake : public Scene {
public:
  void init(Grid& grid) override;

  std::string name() const override {
    return "Melting Snowflake";
  }
};