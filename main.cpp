#include "Eigen/Dense"
#include <fstream>
#include <iostream>
#include <vector>

Eigen::Matrix2d read_2x2_matrix(const char* filepath)
{
    std::ifstream file(filepath);
    std::istream_iterator<double> start(file), end;
    std::vector<double> entries(start, end);
    Eigen::Matrix2d m(2, 2);
    m(0, 0) = entries[0];
    m(0, 1) = entries[1];
    m(1, 0) = entries[2];
    m(1, 1) = entries[3];
    return m;
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        Eigen::Matrix2d lattice = read_2x2_matrix(argv[1]);
        std::cout << lattice << std::endl;
    }
    else
    {
        std::cerr << "No lattice file specified" << std::endl;
        return 1;
    }
    return 0;
}
