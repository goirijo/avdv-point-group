#include "point_group.hpp"
#include <fstream>
#include <iostream>

namespace PointGroupSpace
{
        Eigen::Matrix2d read_lattice(const std::string file_path)
        {
				std::ifstream file;
                file.open(file_path.c_str());
                int cols = 2, rows = 0;
                double data[4];
                while (! file.eof())
                {
                        std::string line;
                        std::getline(file, line);
                        int temp_cols = 0;
                        std::stringstream stream(line);
                        while (! stream.eof())
                        {
                                stream >> data[cols*rows+temp_cols++];
                        }
                        rows++;
                }

                file.close();
                Eigen::Matrix2d lattice;
                for (int i = 0; i < 2; i++)
                        for (int j = 0; j < 2; j++)
                                lattice(i,j) = data[cols*i+j];

                return lattice;
        }
        
        std::vector<std::pair <double, double>> generate_grid(const Eigen::Matrix2d lattice, const std::pair<int,int> a_range, const std::pair<int, int> b_range)
        {
                std::vector<std::pair <double, double>> grid;
                for (int i_a = a_range.first; i_a < a_range.second; i_a++)
                {
                        for (int i_b = b_range.first; i_b < b_range.second; i_b++)
                        {
                                std::pair<double,double> point;
                                point.first = i_a*lattice(0,0) + i_b*lattice(0,1);
                                point.second = i_a*lattice(1,0) + i_b*lattice(1,1);
                                grid.push_back(point);
                        }
                }
                return grid;
        }
        
        std::vector<Eigen::Matrix2d> find_sym_ops(const Eigen::Matrix2d lattice, const std::vector<std::pair <double, double>> grid, double tol)
        {
                std::vector<Eigen::Matrix2d> sym_ops; 
                Eigen::Matrix2d new_lattice;
                for (auto point1 : grid)
                {
                        new_lattice(0,0) = point1.first;
                        new_lattice(1,0) = point1.second;
                        for (auto point2 : grid)
                        {
                                new_lattice(0,1) = point2.first;
                                new_lattice(1,1) = point2.second;
                                auto candidate_sym_op = new_lattice*lattice.inverse();
                                if (sym_op_isValid(candidate_sym_op, tol))
                                        sym_ops.push_back(candidate_sym_op);
                        }
                }
                return sym_ops;
        }

        bool sym_op_isValid(const Eigen::Matrix2d sym_op, double tol)
        {
                bool isValid = true;
                auto size = std::abs(sym_op.determinant());
                if ( std::abs(size - 1) > tol)
                {
                        isValid = false;
                }
                auto skew = sym_op.transpose()*sym_op;
                if (! skew.isApprox(Eigen::Matrix2d::Identity(2,2), tol))
                {
                        isValid = false;
                }
                return isValid;
        }

} // namespace PointGroupSpace


