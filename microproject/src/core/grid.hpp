#pragma once

#include <vector>
#include "core/materials.hpp"

enum class Phase {Solid, Mushy, Liquid};

class Grid {
private:
  int n_;
  double domain_size_;
  double dx_;
  Material mat_;
  std::vector<double> data_;

 public:
  Grid(int n, double domain_size, Material mat);

  double& H(int i, int j);
  double  H(int i, int j) const;

  double T(int i, int j) const;
  Phase phase(int i, int j) const;
  double conductivity(int i, int j) const;

  int size() const {
    return n_;
  }
  double dx() const {
    return dx_;
  }
  double domain_size() const {
    return domain_size_;
  }

  const Material& material() const {
    return mat_;
  }

  double H_from_T_solid(double T_celsius) const;
  double H_from_T_liquid(double T_celsius) const;
};
