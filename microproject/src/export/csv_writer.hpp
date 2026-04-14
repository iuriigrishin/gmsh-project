#pragma once

#include "core/grid.hpp"
#include <string>
#include <fstream>

class CsvWriter {
 private:
  std::ofstream file_;
 public:
  explicit CsvWriter(const std::string& filename);
  ~CsvWriter();

  void write_front(const Grid& grid, double time);

  static void dump_temperature(const Grid& grid, const std::string& filename);
};
