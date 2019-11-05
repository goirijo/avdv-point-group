#include "point_group.hpp"
//#include "Eigen/Dense"
#include <iostream>


int main(int argc, char *argv[])
{
        // Read in lattice
        const std::string file_path = "/home/julija/programming/avdv-point-group/input_lattices/triangular.txt";
        Eigen::Matrix2d lattice = PointGroupSpace::read_lattice(file_path);
        std::cout << "Input lattice:" << std::endl;
        std::cout << lattice << std::endl;
        
        // Make supergrid
        auto a_range = std::make_pair(-2, 3);
        auto b_range = std::make_pair(-2, 3);
        auto grid = PointGroupSpace::generate_grid(lattice, a_range, b_range);
        std::cout << "My grid:" << std::endl;
        for (auto line : grid)
        {
                std::cout << line.first << " " << line.second << std::endl;
        }

// Loop over grid and find symops


// Check if symop is valid and add to list


// Print number of operations in point group, sympos

        return 0;
}
