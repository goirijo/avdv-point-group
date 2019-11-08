#include "Eigen/Dense"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#define PREC 1e-6 // precision for double comparisons

// Read matrix from file
Eigen::Matrix2d read_2x2_matrix(const char* filepath)
{
    std::ifstream file(filepath);
    std::istream_iterator<double> start(file), end;
    std::vector<double> entries(start, end);
    Eigen::Matrix2d m;
    m(0, 0) = entries[0];
    m(0, 1) = entries[1];
    m(1, 0) = entries[2];
    m(1, 1) = entries[3];
    return m;
}

// Generate candidate lattices from a given lattice
std::vector<Eigen::Matrix2d> generate_candidate_lattices(const Eigen::Matrix2d& lattice)
{
    std::vector<Eigen::Matrix2d> candidates;
    int rad = 1; // I think this can always be 1
    for (int i = -rad; i <= rad; i++)
    {
        for (int j = -rad; j <= rad; j++)
        {
            for (int k = -rad; k <= rad; k++)
            {
                for (int l = -rad; l <= rad; l++)
                {
                    Eigen::Matrix2d transf;
                    transf << i, j, k, l;
                    candidates.emplace_back(lattice * transf);
                }
            }
        }
    }
    return candidates;
}

// Check if a symop is valid, i.e. orthogonal
bool is_valid_symop(const Eigen::Matrix2d& symop)
{
    Eigen::Matrix2d id;
    id.setIdentity();
    return id.isApprox(symop * symop.transpose(), PREC);
}

// Generate the point group of a given lattice
std::vector<Eigen::Matrix2d> generate_point_group(const Eigen::Matrix2d& lattice)
{
    std::vector<Eigen::Matrix2d> point_group;
    Eigen::Matrix2d lattice_inv = lattice.inverse();
    for (const auto& lattice_prime : generate_candidate_lattices(lattice))
    {
        // Compute candidate symop and check validity
        const Eigen::Matrix2d symop = lattice_prime * lattice_inv;
        if (is_valid_symop(symop))
        {
            // Check uniqueness against existing symops
            bool unique = true;
            for (const auto& existing : point_group)
            {
                if (symop.isApprox(existing, PREC))
                {
                    unique = false;
                    break;
                }
            }
            if (unique)
            {
                // Add the symop if valid and unique
                point_group.emplace_back(symop);
            }
        }
    }
    return point_group;
}

// Generate the multiplication table for a given point group, determining closure in the process
Eigen::MatrixXi generate_mult_table(const std::vector<Eigen::Matrix2d>& point_group)
{
    const int order = point_group.size();
    Eigen::MatrixXi mult_table(order, order);
    for (int i = 0; i < order; i++)
    {
        for (int j = 0; j < order; j++)
        {
            const Eigen::Matrix2d product = point_group[i] * point_group[j];
            auto it_k = std::find_if(point_group.begin(), point_group.end(),
                                     [&](const Eigen::Matrix2d& symop) { return symop.isApprox(product, PREC); });
            if (it_k != point_group.end())
            {
                mult_table(i, j) = it_k - point_group.begin();
            }
            else
            {
                std::cerr << "Group is not closed" << std::endl;
                return Eigen::MatrixXi::Constant(order, order, -1);
            }
        }
    }
    return mult_table;
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cerr << "No lattice file specified" << std::endl;
        return 1;
    }
    Eigen::Matrix2d lattice = read_2x2_matrix(argv[1]);
    std::cout << "Lattice:" << std::endl << lattice << std::endl << std::endl;
    std::vector<Eigen::Matrix2d> point_group = generate_point_group(lattice);
    std::cout << "Point group has " << point_group.size() << " elements:" << std::endl;
    for (const auto& symop : point_group)
    {
        std::cout << symop << std::endl << std::endl;
    }
    Eigen::MatrixXi mult_table = generate_mult_table(point_group);
    std::cout << "Multiplication table:" << std::endl << mult_table << std::endl;
    return 0;
}
