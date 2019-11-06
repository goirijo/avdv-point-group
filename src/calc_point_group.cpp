#include "../eigen-git-mirror/Eigen/Dense"
#include <list>
#include "headers.h"


std::list<Matrix2f> calc_point_group(const std::list<Matrix2f> lps, const Matrix2f lattice){
    std::list<Matrix2f> SS;
    Matrix2f S;
    for (auto &z: lps){
        S = z*lattice.inverse();
        if (is_sym_op(S)==1){
            SS.push_back(S);
        }
    }
    return SS;
}

