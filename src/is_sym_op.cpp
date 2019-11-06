#include "../eigen-git-mirror/Eigen/Dense"
#include "headers.h"

int is_sym_op(const Matrix2f O){
    if (O.determinant()==1 || O.determinant()==-1){
                Matrix2f t;
                        t = O.transpose()*O;
                        if (t.isIdentity(1e-4)==1){
                                    return 1;

                        }

    }
    return 0;

}


