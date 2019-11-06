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
        /*
        std::cout << "My grid:" << std::endl;
        for (auto line : grid)
        {
                std::cout << line.first << " " << line.second << std::endl;
        } /**/

        // Find and print sym ops
        double tol = 0.00001;
        std::vector<Eigen::Matrix2d> sym_ops = PointGroupSpace::find_sym_ops(lattice, grid, tol);
        std::cout << "Number of sym ops: " << sym_ops.size() << std::endl;
        std::cout << "Sym ops:" << std::endl;
        for ( auto sym_op : sym_ops)
        {
                std::cout << sym_op << "\n" << std::endl;
        }

        return 0;
}
