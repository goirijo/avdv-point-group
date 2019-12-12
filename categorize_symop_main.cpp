#include "Eigen/Core"
#include "Eigen/Eigenvalues"
#include "Eigen/src/Core/Matrix.h"
#include <iostream>
#include <vector>

#define PREC 1e-6

class Symmetry_Operation
{

public:
    Eigen::Matrix3d cart_matrix;
    Eigen::Vector3d translation;

    Symmetry_Operation(Eigen::Matrix3d input_matrix,  Eigen::Vector3d input_translation){
    Eigen::Matrix3d cart_matrix =input_matrix;
    Eigen::Vector3d translation =input_translation;
    };

    

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
 
Eigen::Vector3d project_translation_onto_vector(Eigen::Vector3d  eigen_vector, Eigen::Vector3d translation)
{
    ///projects the translation vectors onto the axis of rotations or mirror plane and returns a vector of this translation
    double projected_component=eigen_vector.dot(translation);
    return projected_component*eigen_vector;

}

Eigen::Vector3d project_translation_onto_mirror(Eigen::MatrixXd eigen_vectors, Eigen::Vector3d translation)
{
    //for each eigen vector this call the above project onto vector function and then added these vectors together
    Eigen::Vector3d projected_translation_vector=Eigen::Vector3d::Zero();
    for(int i=0; i<2;i++){
        Eigen::Vector3d temp_translation_vector =project_translation_onto_vector(eigen_vectors.col(1), translation);
        projected_translation_vector=projected_translation_vector+temp_translation_vector;
    }
    return projected_translation_vector;
}

std::vector<Eigen::Vector3d>  num_eigenvals_equal_one(const Eigen::Matrix3d cart_matrix){
    // some function that evaluate eigne vlaue and counts how many ones
    // Now returns a matrix of the Eigen vectors with eigen values of 1, in column vectors
    // need to edit so that it returns only the real components of the eigenvector
    Eigen::EigenSolver<Eigen::Matrix3d> solver(cart_matrix, true);
    Eigen::Vector3<std::complex<double>> eigenvals = solver.eigenvalues(); 
    Eigen::MatrixXd eigen_vectors;
    std::vector<Eigen::Vector3d> output_eigen_vectors;
    int eigen_counter =0;
    for( int i = 0; i<3; i++){
        if (abs(1-abs(eigenvals(i).real()))<PREC){ 
            if (abs(eigenvals(i).imag())<PREC){
                eigen_counter++;
                Eigen::Vector3d temp;
                eigen_vectors=solver.eigenvectors().col(i);
                std::cout<<eigen_vectors<<std::endl;
                temp=eigen_vectors.real();
                output_eigen_vectors.push_back(temp);
            }
        }
    }
    return output_eigen_vectors;
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
    Eigen::MatrixXd eigen_vectors = num_eigenvals_equal_one(sym_op.cart_matrix);
    std::cout<<eigen_vectors<<std::endl;
    std::cout<<eigen_vectors.cols()<<std::endl;
    if (eigen_vectors.cols() == 2)
    {
        if (has_translation(project_translation_onto_mirror(eigen_vectors, sym_op.translation), lattice)){
                type = "Glide";
                return type;
                }
         else {
             type = "Mirror";
             return type;
         }
    }
    else if (eigen_vectors.cols() == 1)
    {
        if (abs(det-1)<PREC)
        {
            if (has_translation(project_translation_onto_vector(eigen_vectors, sym_op.translation), lattice)){
                type = "Screw";
                return type;
             }
             else
              {
                  type = "Rotation";
                  return type;
              }
         }
         else if(abs(det+1)<PREC) 
         {
            type = "Improper rotation";
            return type;
         }
         else 
         {  
             type = "Error type 1: Type not idenitified!!!";
             return type;
         }
    }
    else
    {
        type = "Error type 2: Type not idenitified!!!";
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
