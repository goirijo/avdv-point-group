#include "eigen-git-mirror/Eigen/Dense"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <ostream>

//Need the factor group for glide planes and screw axes
double find_tau(Eigen::Matrix3f Lattice);
{
  //Find eigenvectors of system. Multipy Eigenvectors by scalar values. If eigenvector applied to lattice points leave it invariant, its one of the top 5 (non-translation based)  operations), if not either no operation or glide/screw axis.
}


//For a given input structure, find:
//the determinant of each symmetry operation (single symop for the function but called multiple times in the main function)
double get_determinant(Eigen::Matrix3f symOp);
{
   return SymOp.determinant(); 
}

double get_trace(Eigen::Matrix3f symOp);
{
   return symOp.trace();
}

int get_number_of_eigenvalues_of_one(Eigen::Matrix3f symOp);
{
   auto eigenvals=symOp.eigenvalues();
   int eigen_counter=0;
   for (eig : eigenvals)
   {
	   if (eig>0.9999 && eig<1.0001)
	   {
		   eigen_counter++;
           }
    }
   return eigen_counter;
}

bool is_tau_lattice_translation(double tau); //this is technically separate from the symMatrix right? Also I'm assuming there's a casm function for grabbing this.


bool is_symop_identity(Eigen::Matrix3f symOp)
{
        if get_trace(symOp)==3 && get_determinant(symOp)==1 && get_number_of_eigenvalues_of_one(SymOp)==3
	{
		return true;
	}
        else
		return false;
		
}

bool is_symop_rotation(Eigen::Matrix3f symOp)
{
        if get_determinant(symOp)==1 && get_number_of_eigenvalues_of_one(SymOp)==1 && is_tau_lattice_translation==true
	{
		return true;
	}
        else
		return false;
}

bool is_symop_improper_rotation(Eigen::Matrix3f symOp);//may combine improper rot and mirror plane
{
	if get_determinant(symOp)==-1 && get_number_of_eigenvalues_of_one(SymOp)==1
	{
		return true;
	}
        else
		return false;
}

bool is_symop_mirror_plane(Eigen::Matrix3f symOp);
{
        if get_determinant(symOp)==-1 && get_number_of_eigenvalues_of_one(SymOp)==2 && is_tau_lattice_translation==true
	{
		return true;
	}
        else
		return false;
}

bool is_symop_inversion(Eigen::Matrix3f symOp);
{
	if get_trace==-3 && get_determinant(symOp)==-1 && get_number_of_eigenvalues_of_one(SymOp)==0
	{
		return true;
	}
        else
		return false;
}

bool is_symop_screw_axis(Eigen::Matrix3f symOp, double tau);
{
        if get_determinant(symOp)==1 && get_number_of_eigenvalues_of_one(SymOp)==1 && is_tau_lattice_translation==false
	{
		return true;
	}
        else
		return false;
}

bool is_symop_glide_plane(Eigen::Matrix3f symOp, double tau);
{
        if get_determinant(symOp)==-1 && get_number_of_eigenvalues_of_one(SymOp)==2 && is_tau_lattice_translation==false
	{
		return true;
	}
        else
		return false;
}


int main()
{



	//need check for if there are any symops that are considered both. If so, something must check for this and let us know as this means we have an error in our midsts
}
