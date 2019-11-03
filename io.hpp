#ifndef IO_HH
#define IO_HH

#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include "Eigen/Dense"

namespace io
{
Eigen::Matrix2d read_matrix2d_from_file(const std::string& file_path);

template<typename T>
void print_formatted_table(std::ostream& stream, const std::vector<std::vector<T>>& values, int width)
{
    for(const auto& row : values)
    {
        for(const auto& val : row)
        {
            stream<<std::setw(width)<<val;
        }
        stream<<std::endl;
    }
    return;
}
} // namespace io


#endif
