#pragma once
namespace sim {
struct SimMaterial {
  double k_solid   = 2.2;       // теплопроводность льда, Вт/(м*К)
  double k_liquid  = 0.6;       // теплопроводность воды, Вт/(м*К)
  double c_solid   = 2090.0;    // теплоёмкость льда, Дж/(кг*К)
  double c_liquid  = 4180.0;    // теплоёмкость воды, Дж/(кг*К)
  double L         = 334000.0;  // теплота плавления льда, Дж/кг
  double T_melt    = 0.0;       // температура плавления, Цельсий
  double rho       = 1000.0;    // плотность, кг/м^3
};

}