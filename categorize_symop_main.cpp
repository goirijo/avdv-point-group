#include "Eigen/Core"
#include "Eigen/Eigenvalues"
#include "Eigen/src/Core/Matrix.h"
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

#define PREC 1e-6

class Symmetry_Operation
{

public:
    Eigen::Matrix3d cart_matrix;
    Eigen::Vector3d translation;

    Symmetry_Operation(Eigen::Matrix3d input_matrix, Eigen::Vector3d input_translation)
    {
        Eigen::Matrix3d cart_matrix = input_matrix;
        Eigen::Vector3d translation = input_translation;
    }
};

bool compare_diff_to_prec(Eigen::Vector3d difference)
{
    int sig_diff = 0;
    for (int i = 0; i < 3; i++)
    {
        if (abs(difference(1)) > PREC)
        {
            sig_diff++;
        }
    }
    if (sig_diff != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool has_translation(const Eigen::Vector3d translation, const Eigen::Matrix3d lattice)
{ // check if translation is 0 or integer multiple of lattice vectors-> false,
    // else true
    Eigen::Vector3d lattice_displacement = lattice.inverse() * translation;
    Eigen::Vector3d modification(0.5, 0.5, 0.5);
    Eigen::Vector3d modif_displ = lattice_displacement + modification;
    Eigen::Vector3d trunc_displ;

    for (int i = 0; i < 3; i++)
    {
        trunc_displ(i) = (int)modif_displ(i);
    }
    Eigen::Vector3d difference = lattice_displacement - trunc_displ;
    bool check_trans = compare_diff_to_prec(difference);

    if (check_trans == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Eigen::Vector3d project_translation_onto_vectors(const std::vector<Eigen::Vector3d>& eigen_vectors, Eigen::Vector3d translation)
{
    // for each eigen vector this call the above project onto vector function and then added these vectors together
    Eigen::Vector3d projected_translation_vector = Eigen::Vector3d::Zero();
    for (const auto& vector : eigen_vectors)
    {
        double projected_component = vector.dot(translation);
        projected_translation_vector += (projected_component * vector);
    }
    return projected_translation_vector;
}

bool almost_equal(double LHS, double RHS, double eps = PREC) { return std::abs(LHS - RHS) < eps; }

/**
 * Returns a container of eigenvectors of the given matrix that have
 * positive unit eigenvalues
 */

std::vector<Eigen::Vector3d> eigenvectors_with_positive_unit_eigenvalues(const Eigen::Matrix3d& cart_matrix)
{
    // some function that evaluate eigne vlaue and counts how many ones
    // Now returns a matrix of the Eigen vectors with eigen values of 1, in column vectors
    // need to edit so that it returns only the real components of the eigenvector
    Eigen::EigenSolver<Eigen::Matrix3d> solver(cart_matrix, true);
    Eigen::Vector3<std::complex<double>> eigenvals = solver.eigenvalues();
    std::vector<Eigen::Vector3d> output_eigen_vectors;
    Eigen::Matrix3<std::complex<double>> eigenvectors = solver.eigenvectors();

    for (int i = 0; i < 3; i++)
    {
        auto eigenval = eigenvals(i);
        if (almost_equal(eigenval.real(), 1))
        {
            std::cout << eigenvectors.col(i) << std::endl;
            Eigen::Vector3d real_eigenvector = eigenvectors.col(i).real();
            output_eigen_vectors.push_back(real_eigenvector);
        }
    }

    return output_eigen_vectors;
}

std::string check_op_type(const Symmetry_Operation sym_op, const Eigen::Matrix3d lattice)
{ // take in sym_op returns string of op type
    int trace = sym_op.cart_matrix.trace();
    std::string type;
    double det = sym_op.cart_matrix.determinant();

    if ((3 - trace < PREC))
    {
        type = "Identity";
        return type;
    }
    if (trace + 3 < PREC)
    {
        type = "Inversion";
        return type;
    }
    std::vector<Eigen::Vector3d> eigen_vectors = eigenvectors_with_positive_unit_eigenvalues(sym_op.cart_matrix);
    /* std::cout << eigen_vectors << std::endl; */
    /* std::cout << eigen_vectors.cols() << std::endl; */
    if (eigen_vectors.size() == 2)
    {
        if (has_translation(project_translation_onto_vectors(eigen_vectors, sym_op.translation), lattice))
        {
            type = "Glide";
            return type;
        }
        else
        {
            type = "Mirror";
            return type;
        }
    }
    else if (eigen_vectors.size() == 1)
    {
        if (abs(det - 1) < PREC)
        {
            if (has_translation(project_translation_onto_vectors(eigen_vectors, sym_op.translation), lattice))
            {
                type = "Screw";
                return type;
            }
            else
            {
                type = "Rotation";
                return type;
            }
        }
        else if (abs(det + 1) < PREC)
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
    some_translation << 0.0, 0.0, 0.5;
    Eigen::Matrix3d ymirror;
    ymirror << 1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0;
    ::Symmetry_Operation sym_op(ymirror, some_translation);

    Eigen::Matrix3d lattice;
    lattice << 3.5, 0.0, 0.0, 0.0, 3.5, 0.0, 0.0, 0.0, 4.0;
    std::string op_type = check_op_type(sym_op, lattice);
    std::cout << "This is a " << op_type << std::endl;
    return 0;
}
