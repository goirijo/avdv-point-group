#ifndef XTAL_HH
#define XTAL_HH

#include <vector>
#include "Eigen/Dense"

namespace xtal
{

typedef Eigen::Vector2d LatticePoint;
typedef Eigen::Vector2d LatticeVector;

//---- c++ concept: Classes combine data and related methods into an object ----//
class Lattice
{
public:
    //---- c++ concept: Classes usually require constructors ----//
    Lattice(const LatticeVector& init_a, const LatticeVector& init_b)
        : m_vectors_as_columns_matrix(Lattice::vertical_stack(init_a, init_b))
    {
    }

    /// Initialize a lattice by reading the column vectors from a file
    static Lattice from_file(const std::string& filename);

    const Eigen::Matrix2d& vectors_as_columns() const { return this->m_vectors_as_columns_matrix; }

    LatticeVector a() const { return this->vectors_as_columns().col(0); }

    LatticeVector b() const { return this->vectors_as_columns().col(1); }

private:
    Lattice(const Eigen::Matrix2d& init_vectors_as_columns_matrix)
        : m_vectors_as_columns_matrix(init_vectors_as_columns_matrix)
    {
    }

    Eigen::Matrix2d m_vectors_as_columns_matrix;

    //Given two column vectors, create a matrix where they are aligned side by side
    static Eigen::Matrix2d vertical_stack(const LatticeVector& a, const LatticeVector& b);
};

/// Return a vector of lattice points by extending the lattice maxsearch times in all directions.
/// This is basically just a list of lattice points within a certain radius of the origin,
/// where the radius is specified by number of unit cells.
std::vector<LatticePoint> lattice_points_in_radius(const Lattice& lat, int search_radius);
} // namespace xtal


#endif
