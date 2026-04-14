#include "export/csv_writer.hpp"

CsvWriter::CsvWriter(const std::string& filename): file_(filename)
{
  file_ << "time,front_x\n";
}

CsvWriter::~CsvWriter() {
  file_.close();
}

void CsvWriter::write_front(const Grid& grid, double time) {
  int n = grid.size();
  int mid = n / 2;

  double front_x = -1.0;
  for (int j = 0; j < n - 1; ++j) {
    Phase here = grid.phase(mid, j);
    Phase next = grid.phase(mid, j + 1);
    if (here == Phase::Solid && next != Phase::Solid) {
      front_x = (j + 0.5) * grid.dx();
      break;
    }
    if (here != Phase::Solid && next == Phase::Solid) {
      front_x = (j + 0.5) * grid.dx();
      break;
    }
  }

  if (front_x > 0.0) {
    file_ << time << "," << front_x << "\n";
  }
}

void CsvWriter::dump_temperature(const Grid& grid, const std::string& filename) {
  std::ofstream f(filename);
  int n = grid.size();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (j > 0) f << ",";
      f << grid.T(i, j);
    }
    f << "\n";
  }
}
