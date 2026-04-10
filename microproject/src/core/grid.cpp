#include "core/grid.hpp"

Grid::Grid(int n, double domain_size, sim::SimMaterial material) : n_(n), domain_size_(domain_size),
  dx_(domain_size / n), mat_(material), data_(n * n, {0.0}) {}

double& Grid::H(int i, int j) {
  return data_[i * n_ + j];
}

double Grid::H(int i, int j) const {
  return data_[i * n_ + j];
}

double Grid::T(int i, int j) const {
    double h = H(i, j);
    double rhoL = mat_.rho * mat_.L;
    if (h < 0.0) {
      return mat_.T_melt + h / (mat_.rho * mat_.c_solid);
    } else if (h > rhoL) {
      return mat_.T_melt + (h - rhoL) / (mat_.rho * mat_.c_liquid);
    } else {
      return mat_.T_melt;
    }
}

Phase Grid::phase(int i, int j) const {
  double h = H(i, j);
  double rhoL = mat_.rho * mat_.L;

  if (h < 0.0) {
		return Phase::Solid;
	}
  if (h > rhoL) {
		return Phase::Liquid;
	}
  return Phase::Mushy;
}

double Grid::conductivity(int i, int j) const {
  double h = H(i, j);
  double rhoL = mat_.rho * mat_.L;

  if (h < 0.0) {
		return mat_.k_solid;
	}
  if (h > rhoL) {
		return mat_.k_liquid;
	}

  double frac = h / rhoL;
  return (1.0 - frac) * mat_.k_solid + frac * mat_.k_liquid;
}

double Grid::H_from_T_solid(double T_celsius) const {
  return mat_.rho * mat_.c_solid * (T_celsius - mat_.T_melt);
}

double Grid::H_from_T_liquid(double T_celsius) const {
  return mat_.rho * mat_.L + mat_.rho * mat_.c_liquid * (T_celsius - mat_.T_melt);
}
