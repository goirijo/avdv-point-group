#include "Eigen/Dense"
#include <fstream>
#include <iostream>
#include <vector>

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

// Generate candidate lattices from a given lattice and "radius"
std::vector<Eigen::Matrix2d> generate_candidate_lattices(const Eigen::Matrix2d& lattice, const int& rad)
{
    std::vector<Eigen::Matrix2d> candidates;
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
    return id.isApprox(symop * symop.transpose());
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cerr << "No lattice file specified" << std::endl;
        return 1;
    }
    Eigen::Matrix2d lattice = read_2x2_matrix(argv[1]);
    std::vector<Eigen::Matrix2d> candidates = generate_candidate_lattices(lattice, 1);
    return 0;
}
