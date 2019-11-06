#ifndef POINTGROUP_HH
#define POINTGROUP_HH

#include "Eigen/Dense"
#include <vector>

namespace PointGroupSpace
{
        // reads in file with 2 by 2 values as a 2 by 2 matrix of doubles
        Eigen::Matrix2d read_lattice(const std::string file_path);

        // given grid dimensions, generates supergrid
        std::vector<std::pair <double, double>> generate_grid(const Eigen::Matrix2d lattice, const std::pair<int,int> a_range, const std::pair<int, int> b_range);

        // loops over grid points and returns valid sym ops
        std::vector<Eigen::Matrix2d> find_sym_ops(const Eigen::Matrix2d lattice, const std::vector<std::pair <double, double>> grid, double tol);
        
        // checks that sym op keeps volume same and does not skew
        bool sym_op_isValid(const Eigen::Matrix2d sym_op, const double tol);
       
} // PointGroupSpace


#endif
