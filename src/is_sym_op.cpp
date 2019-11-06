#include "../eigen-git-mirror/Eigen/Dense"
#include "headers.h"

int is_sym_op(const Matrix2f O){
    float pp = O.determinant();
    if (pp==1 || pp==-1){
        Matrix2f t;
        t = O.transpose()*O;
    if (t.isIdentity(1e-4)==1){
        return 1;
    }
    }
    return 0;
}


