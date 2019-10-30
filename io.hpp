#ifndef IO_HH
#define IO_HH

#include <string>
#include <vector>
#include <fstream>
#include "Eigen/Dense"

namespace io
{
Eigen::Matrix2d read_lattice_from_file(const std::string& file_path);
} // namespace io


#endif
