#include "math.hpp"
#include "definitions.hpp"
#include <cmath>

namespace math
{
bool almost_equal(double val1, double val2) { return std::abs(val1 - val2) < EPS; }

bool almost_equal(const Eigen::MatrixXd& mat1, const Eigen::MatrixXd& mat2) { return mat1.isApprox(mat2, EPS); }

bool matrix_determinant_magnitude_is_unity(const Eigen::MatrixXd& mat)
{
    return almost_equal(std::abs(mat.determinant()), 1);
}

// A matrix is unitary if the conjugate transpose is also the inverse
bool matrix_is_unitary(const Eigen::MatrixXd& mat)
{
    auto identity = Eigen::MatrixXd::Identity(mat.rows(), mat.cols());
    return almost_equal(mat * mat.transpose(), identity);
}

int angle_between_vectors(const Eigen::Vector2d& vec1, const Eigen::Vector2d& vec2)
{
    auto radians = std::atan2(vec2(1), vec2(0)) - std::atan2(vec1(1), vec1(0));
    return std::round(radians * (180.0 / M_PI));
}
} // namespace math
