#ifndef HEADERS_H
#define HEADERS_H
#include <list>

typedef Eigen::Matrix<double,2,2> Matrix2f;
typedef Eigen::Matrix<double,2,1> Vector2f;

int is_sym_op(const Matrix2f O);
std::list<Vector2f> calc_grid_points(int radius, const Matrix2f lattice);
std::list<Matrix2f> calc_l_primes(const std::list<Vector2f> ps);
std::list<Matrix2f> calc_point_group(const std::list<Matrix2f> lps, const Matrix2f lattice);

#endif
