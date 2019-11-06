#include "../eigen-git-mirror/Eigen/Dense"
#include "headers.h"

int is_sym_op(const Matrix2f O){
    if (approximatelyEqualAbsRel(O.determinant(),1,1e-6,1e-6)==true||approximatelyEqualAbsRel(O.determinant(),-1,1e-6,1e-6)==true){
        Matrix2f t;
        t = O.transpose()*O;
    if (t.isIdentity(1e-4)==1){
        return 1;
    }
    }
    return 0;
}


