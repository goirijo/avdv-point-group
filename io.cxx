#include <iterator>
#include "io.hpp"

namespace io
{
std::vector<double> _unrolled_matrix_from_file(const std::string& file_path)
{
    std::ifstream file_stream(file_path);
    std::istream_iterator<double> file_start(file_stream),file_end;
    std::vector<double> lat_values(file_start,file_end);

    return lat_values;
}

Eigen::Matrix2d read_matrix2d_from_file(const std::string& file_path)
{
    auto lat_values = _unrolled_matrix_from_file(file_path);
    Eigen::Matrix2d lattice;
    lattice(0, 0) = lat_values[0];
    lattice(0, 1) = lat_values[1];
    lattice(1, 0) = lat_values[2];
    lattice(1, 1) = lat_values[3];

    return lattice;
}
} // namespace io

