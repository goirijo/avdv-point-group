#include "xtal.hpp"
#include "io.hpp"

namespace xtal
{
Lattice Lattice::from_file(const std::string& filename)
{
    auto column_matrix = io::read_matrix2d_from_file(filename);
    return Lattice(column_matrix);
}

Eigen::Matrix2d Lattice::vertical_stack(const LatticeVector& a, const LatticeVector& b)
{
    Eigen::Matrix2d mat;
    mat(0, 0) = a(0);
    mat(1, 0) = a(1);
    mat(0, 1) = b(0);
    mat(1, 1) = b(1);

    return mat;
}

/// Return a vector of lattice points by extending the lattice maxsearch times in all directions.
/// This is basically just a list of lattice points within a certain radius of the origin,
/// where the radius is specified by number of unit cells.
std::vector<LatticePoint> lattice_points_in_radius(const Lattice& lat, int search_radius)
{
    auto a_vector = lat.a();
    auto b_vector = lat.b();

    std::vector<LatticePoint> lattice_points;
    for (int i = -search_radius; i <= search_radius; ++i)
    {
        for (int j = -search_radius; j <= search_radius; ++j)
        {
            auto point = i * a_vector + j * b_vector;
            lattice_points.push_back(point);
        }
    }

    return lattice_points;
}
} // namespace xtal
