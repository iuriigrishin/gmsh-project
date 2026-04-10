#pragma once

#include <string>
#include "core/grid.hpp"

class Scene {
public:
    virtual ~Scene() = default;
    virtual void init(Grid& grid) = 0;
    virtual std::string name() const = 0;
};
