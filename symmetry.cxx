#include "symmetry.hpp"
#include "math.hpp"
#include "xtal.hpp"
#include <iostream>

namespace sym
{

Operation Operation::operator*(const Operation& other) const
{
    auto product_mat = this->cartesian_matrix() * other.cartesian_matrix();
    return Operation(product_mat);
}

/// Given a symmetry operation, determine what type of operation it is
/// by inspecting its determinant
Operation::TYPE Operation::categorize(const OperationMatrix& operation)
{
    auto identity = Eigen::MatrixXd::Identity(operation.rows(), operation.cols());

    if (math::almost_equal(identity, operation))
    {
        return TYPE::IDENTITY;
    }

    auto determinant = operation.determinant();

    if (math::almost_equal(determinant, 1))
    {
        return TYPE::ROTATION;
    }

    if (math::almost_equal(determinant, -1))
    {
        return TYPE::MIRROR;
    }

    return TYPE::UNDEFINED;
}

/// Given a symmetry operation matrix, determine the angle associated with it, e.g.
/// angle or rotation or mirror, rounded to int (0-360)
int Operation::calculate_angle(const OperationMatrix& operation)
{
    Eigen::Vector2d ref_vec;
    ref_vec << 1, 0;

    auto transformed_vec = operation * ref_vec;
    auto angle = math::angle_between_vectors(ref_vec, transformed_vec);
    auto positive_angle = (360 + angle) % 360;
    return positive_angle;
}

/// Given a symmetry operation, return a string that identifies it by type of operation,
/// and the associated angle of rotation/reflection
std::string Operation::make_label(const OperationMatrix& operation)
{
    auto type = categorize(operation);
    auto angle = calculate_angle(operation);
    auto combine_to_string = [](const std::string& prefix, int angle) { return prefix + std::to_string(angle); };

    std::string label;
    switch (type)
    {
    case TYPE::IDENTITY:
        label = combine_to_string("I", angle);
        break;
    case TYPE::ROTATION:
        label = combine_to_string("R", angle);
        break;
    case TYPE::MIRROR:
        label = combine_to_string("M", angle);
        break;
    case TYPE::UNDEFINED:
        label = "UNDEFINED";
        break;
    default:
        std::cerr << "Your symmetry operation coudn't be properly categorized. There's no recovering from this"
                  << std::endl;
        exit(1);
    }

    return label;
}

std::ostream& operator<<(std::ostream& stream, const Operation& op)
{
    stream<<op.label();
    return stream;
}

/// Returns the point group of the given Lattice, i.e. the group of symmetry operations
/// that maps the lattice onto itself.
std::set<Operation> point_group(const xtal::Lattice& lattice)
{
    std::set<Operation> point_group_operations;

    // This radius should be more than enough
    auto lattice_points = xtal::lattice_points_in_radius(lattice, 5);

    for (const auto& p1 : lattice_points)
    {
        for (const auto& p2 : lattice_points)
        {
            // Create a new lattice from the two points.
            // Each of the points effectively defines the a and b vectors of the
            // new lattice
            xtal::Lattice possible_transformed_lattice(p1, p2);
            // Find the transformation that takes you from the original lattice
            // to this new lattice. The result is a candidate symmetry operation
            const auto& orig_lat_mat = lattice.vectors_as_columns();
            const auto& possible_lat_mat = possible_transformed_lattice.vectors_as_columns();
            auto possible_symmetry_operation = possible_lat_mat * (orig_lat_mat.inverse());

            // The candidate operation is only valid if its determinant magnitude is unity
            // AND it is unitary
            if (math::matrix_determinant_magnitude_is_unity(possible_symmetry_operation) &&
                math::matrix_is_unitary(possible_symmetry_operation))
            {
                point_group_operations.emplace(possible_symmetry_operation);
            }
        }
    }

    return point_group_operations;
}

bool group_is_colsed(const std::set<Operation>& sym_group)
{
    for (const auto op1 : sym_group)
    {
        for (const auto op2 : sym_group)
        {
            auto product = op1 * op2;
            if (sym_group.find(product) == sym_group.end())
            {
                return false;
            }
        }
    }
    return true;
}

std::vector<std::vector<Operation>> make_multiplication_table(const std::set<Operation>& sym_group)
{
    std::vector<Operation> unrolled_group(sym_group.begin(),sym_group.end());

    int num_ops=unrolled_group.size();
    std::vector<std::vector<Operation>> multiplication_table(num_ops, std::vector<Operation>(num_ops,Operation::undefined()));

    for (int i = 0; i < unrolled_group.size(); ++i)
    {
        for (int j = 0; j < unrolled_group.size(); ++j)
        {
            Operation product=unrolled_group[i]*unrolled_group[j];
            multiplication_table[i][j]=product;
        }
    }

    return multiplication_table;
}

} // namespace sym

