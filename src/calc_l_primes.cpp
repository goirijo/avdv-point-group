#include "../eigen-git-mirror/Eigen/Dense"
#include <list>
#include "headers.h"


std::list<Matrix2f> calc_l_primes(const std::list<Vector2f> ps){
    Matrix2f m;
    std::list<Matrix2f> lprime;
    for (auto &v : ps){
        for (auto &w : ps){
            m(0,0) = v(0,0);
            m(0,1) = w(0,0);
            m(1,0) = v(1,0);
            m(1,1) = w(1,0);
            lprime.push_back(m);
        }
    }
    return lprime;
}

