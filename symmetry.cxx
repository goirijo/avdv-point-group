#include <iostream>
#include "symmetry.hpp"
#include "math.hpp"
#include "xtal.hpp"

namespace sym
{

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
    default:
        std::cerr << "Your symmetry operation coudn't be properly categorized. There's no recovering from this"
                  << std::endl;
        exit(1);
    }

    return label;
}

/// Returns the point group of the given Lattice, i.e. the group of symmetry operations
/// that maps the lattice onto itself.
std::vector<Operation> point_group(const xtal::Lattice& lattice)
{
    std::vector<Operation> point_group_operations;

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
                point_group_operations.emplace_back(possible_symmetry_operation);
            }
        }
    }

    return point_group_operations;
}

} // namespace sym

