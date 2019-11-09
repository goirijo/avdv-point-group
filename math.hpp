#ifndef MATH_HH
#define MATH_HH

#include "Eigen/Dense"

//---- c++ concept: Namespaces can group conceptually related functions and classes together ----//
namespace math
{
bool almost_equal(double val1, double val2);

bool almost_equal(const Eigen::MatrixXd& mat1, const Eigen::MatrixXd& mat2);

// A matrix is unitary if the conjugate transpose is also the inverse
bool is_orthogonal(const Eigen::MatrixXd& mat);

int angle_between_vectors(const Eigen::Vector2d& vec1, const Eigen::Vector2d& vec2);
} // namespace math

#endif
