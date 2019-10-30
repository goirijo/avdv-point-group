#include "Eigen/Dense"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace io
{
std::vector<std::string> _parse_lattice_file(const std::string& file_path)
{
    std::ifstream file_stream;
    file_stream.open(file_path.c_str());

    std::vector<std::string> lat_values;
    std::string value;
    while (file_stream >> value)
    {
        lat_values.push_back(value);
    }

    return lat_values;
}

Eigen::Matrix2d read_lattice_from_file(const std::string& file_path)
{
    auto lat_values = _parse_lattice_file(file_path);
    Eigen::Matrix2d lattice;
    lattice(0, 0) = std::stod(lat_values[0]);
    lattice(0, 1) = std::stod(lat_values[1]);
    lattice(1, 0) = std::stod(lat_values[2]);
    lattice(1, 1) = std::stod(lat_values[3]);

    return lattice;
}
} // namespace io

namespace xtal
{

typedef Eigen::Vector2d LatticePoint;

class Lattice
{
public:
    Lattice(const Eigen::Matrix2d& init_vectors_as_columns_matrix)
        : vectors_as_columns_matrix(init_vectors_as_columns_matrix)
    {
    }

    Eigen::Vector2d a() const { return this->vectors_as_columns_matrix.col(0); }

    Eigen::Vector2d b() const { return this->vectors_as_columns_matrix.col(1); }

private:
    Eigen::Matrix2d vectors_as_columns_matrix;
};


} // namespace xtal

int main(int argc, char* argv[])
{
    std::string lat_file(argv[1]);
    auto lattice_mat = io::read_lattice_from_file(lat_file);

    xtal::Lattice lattice(lattice_mat);

    std::cout << "a:    " << lattice.a().transpose() << std::endl;
    std::cout << "b:    " << lattice.b().transpose() << std::endl;
    return 0;
}
