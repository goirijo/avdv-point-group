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


// Loop over grid and find symops


// Check if symop is valid and add to list


// Print number of operations in point group, sympos

        return 0;
}
