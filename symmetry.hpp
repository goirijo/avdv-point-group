#ifndef SYMMETRY_HH
#define SYMMETRY_HH

#include "Eigen/Dense"
#include <set>
#include <vector>

namespace xtal
{
class Lattice;
}

namespace sym
{
typedef Eigen::Matrix2d OperationMatrix;

class Operation
{
public:
    /// Enum class to categorize operations as identity, rotation, or reflection
    enum class TYPE
    {
        IDENTITY,
        ROTATION,
        MIRROR,
        UNDEFINED
    };

    Operation(const OperationMatrix& cartesian_matrix)
        : m_cartesian_matrix(cartesian_matrix), operation_label(make_label(cartesian_matrix))
    {
    }

    static Operation undefined() { return Operation(OperationMatrix::Zero()); }

    const OperationMatrix& cartesian_matrix() const { return this->m_cartesian_matrix; }

    /// Return label that identifies the symmetry operation as identity, rotation, or mirror, as well
    /// as the angle associated with the operation
    std::string label() const { return this->operation_label; }

    /// Result is simply a direct comparison of the label
    bool operator<(const Operation& other) const { return this->label() < other.label(); }

    /// Take the product of two operations by multiplying the cartesian matrix
    Operation operator*(const Operation& other) const;

private:
    OperationMatrix m_cartesian_matrix;

    std::string operation_label;

    /// Given a symmetry operation, determine what type of operation it is
    /// by inspecting its determinant
    static TYPE categorize(const OperationMatrix& operation);

    /// Given a symmetry operation matrix, determine the angle associated with it, e.g.
    /// angle or rotation or mirror, rounded to int (0-360)
    static int calculate_angle(const OperationMatrix& operation);

    /// Given a symmetry operation, return a string that identifies it by type of operation,
    /// and the associated angle of rotation/reflection
    static std::string make_label(const OperationMatrix& operation);
};

/// Returns the point group of the given Lattice, i.e. the group of symmetry operations
/// that maps the lattice onto itself.
std::set<Operation> make_point_group(const xtal::Lattice& lattice);

/// Ensure that the symmetry group is closed by multiplying every element by every element
/// and checking that the result is alrady in the group
bool group_is_colsed(const std::set<Operation>& sym_group);

/// Construct multiplication table of group by storing pointers to each element in the symmetry group
/// in a two dimensional vector. Group must be closed!
std::vector<std::vector<Operation>> make_multiplication_table(const std::set<Operation>& sym_group);

/// Outputs the label of the operation
std::ostream& operator<<(std::ostream& stream, const Operation& op);

} // namespace sym

#endif
