#include "../eigen-git-mirror/Eigen/Dense"
#include<cmath>
#include "headers.h"

bool approximatelyEqualAbsRel(double a, double b, double absEpsilon, double relEpsilon){
// Check if the numbers are really close -- needed when comparing numbers near zero.
    double diff{ fabs(a - b)  };
    if (diff <= absEpsilon)
        return true;
        //                      // Otherwise fall back to Knuth's algorithm
        return diff <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * relEpsilon );
        //
}

