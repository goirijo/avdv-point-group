#include "Eigen/Core"
#include "Eigen/Eigenvalues"
#include "Eigen/src/Core/Matrix.h"
#include <iostream>
#include <vector>

#define PREC 1e-6

class Symmetry_Operation
{

public:

    Symmetry_Operation(Eigen::Matrix3d cart_matrix,  Eigen::Vector3d translation);
    Eigen::Matrix3d cart_matrix;
    Eigen::Vector3d translation;

    

};

bool compare_diff_to_prec(Eigen::Vector3d difference) {
    int sig_diff = 0;
    for(int i=0;i<3;i++){if (abs(difference(1)) > PREC){sig_diff++;}}
        if (sig_diff != 0) {
            return true;
         }
         else {
           return false;
         }
 }
 
 bool has_translation(const Eigen::Vector3d translation,const Eigen::Matrix3d lattice)
{// check if translation is 0 or integer multiple of lattice vectors-> false,
    // else true
    Eigen::Vector3d lattice_displacement = lattice.inverse() * translation;
    Eigen::Vector3d modification(0.5, 0.5, 0.5);
    Eigen::Vector3d modif_displ = lattice_displacement + modification;
    Eigen::Vector3d trunc_displ;
    
    for( int i=0; i<3; i++){trunc_displ(i)= (int) modif_displ(i);}
    Eigen::Vector3d difference = lattice_displacement - trunc_displ;
    bool check_trans = compare_diff_to_prec(difference);
    
    if (check_trans == true) {
        return true;
    }
    else {
        return false;
    }
 }
 

int num_eigenvals_equal_one(const Eigen::Matrix3d cart_matrix){
    // some function that evaluate eigne vlaue and counts how many ones
    Eigen::EigenSolver<Eigen::Matrix3d> solver(cart_matrix, false);
    Eigen::Vector3<std::complex<double>> eigenvals = solver.eigenvalues(); 
    int eigen_counter =0;
    for( int i = 0; i<3; i++){
        if (abs(1-eigenvals(i).real())<PREC){ 
            if (abs(eigenvals(i).imag())<PREC){eigen_counter++;
            }}
    }
    return eigen_counter;
    /// det (matrix -lambda*identity)==0, lambda is the eigenvalues
}

std::string check_op_type(const Symmetry_Operation sym_op, const Eigen::Matrix3d lattice)
{ // take in sym_op returns string of op type
    int trace = sym_op.cart_matrix.trace();
    std::string type;
    double det = sym_op.cart_matrix.determinant();

    if ((3-trace <PREC))
    {
        type = "Identity";
        return type;
    }
    if (trace + 3 < PREC)
    {
        type = "Inversion";
        return type;
    }
    if (has_translation(sym_op.translation, lattice))
    {
        if (abs(1-det)<PREC)
        {
            type = "Screw";
            return type;
        }
        else
        {
            type = "Glide";
            return type;
        }
    }
    if (abs(det-1)<PREC)
    {
        type = "rotation";
        return type;
    }
    int total_eigenvals_equal_one = num_eigenvals_equal_one(sym_op.cart_matrix);
    if (total_eigenvals_equal_one == 2)
    {
        type = "Mirror";
        return type;
    }
    else if (total_eigenvals_equal_one == 1)
    {
        type = "Improper rotation";
        return type;
    }
    else
    {
        type = "Error: Type not idenitified!!!";
        return type;
    }
}

int main()
{ // WHAT is the actual input????
    // for now, hard coding example sym_ops

    Eigen::Vector3d some_translation;
    some_translation<<0.0,0.0,0.5;
    Eigen::Matrix3d ymirror;
    ymirror << 1.0, 0.0, 0.0,
               0.0, -1.0, 0.0,
               0.0, 0.0, 1.0;
    ::Symmetry_Operation sym_op(ymirror,some_translation);

    Eigen::Matrix3d lattice;
    lattice<< 3.5, 0.0, 0.0,
              0.0, 3.5, 0.0,
              0.0, 0.0, 4.0;
    std::string op_type = check_op_type(sym_op, lattice);
    std::cout << "This is a " << op_type << std::endl;
    return 0;
}
