#include "../eigen-git-mirror/Eigen/Dense"
#include <list>
#include "headers.h"

std::list<Vector2f> calc_grid_points(int radius, const Matrix2f lattice){

    Vector2f a,b;
    int i,j;

    a << lattice(0,0), lattice(1,0);
    b << lattice(0,1), lattice(1,1);

    std::list<Vector2f> p;
    for (i=-radius; i<=radius; i++){
        for (j=-radius; j<=radius; j++){
            p.push_back({i*a(0,0)+j*b(0,0), i*a(1,0)+j*b(1,0)});
        }
    }
return p;
}


