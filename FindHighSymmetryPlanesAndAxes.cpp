#include "eigen-git-mirror/Eigen/Dense"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <ostream>
#include "main.cpp"

//Need the factor group for glide planes and screw axes
double find_tau(Eigen::Matrix3f Lattice) //find point group operations of lattice, candidates, guess all possible translations (need structure due to bases)
{
  //Find eigenvectors of system. Multipy Eigenvectors by scalar values. If eigenvector applied to lattice points leave it invariant, its one of the top 5 (non-translation based)  operations), if not either no operation or glide/screw axis.
}


//For a given input structure, find:
//the determinant of each symmetry operation (single symop for the function but called multiple times in the main function)
double get_determinant(Eigen::Matrix3f symOp)
{
   return symOp.determinant(); 
}

double get_trace(Eigen::Matrix3f symOp)
{
   return symOp.trace();
}

int get_number_of_eigenvalues_of_one(Eigen::Matrix3f symOp)
{
   auto eigenvals=symOp.eigenvalues();
   int eigen_counter=0;
   for (int i=0; i<eigenvals.size(); i++)  //This might not work as inteneded
   {
	   if (i==1)
	   {
		   eigen_counter++;
           }
    }
   return eigen_counter;
}

//bool is_tau_lattice_translation(double tau); //this is technically separate from the symMatrix right? Also I'm assuming there's a casm function for grabbing this.


bool is_symop_identity(Eigen::Matrix3f symOp)
{
        if (get_trace(symOp)==3 && get_determinant(symOp)==1 && get_number_of_eigenvalues_of_one(symOp)==3)
	{
		return true;
	}
        else
		return false;
		
}

bool is_symop_rotation(Eigen::Matrix3f symOp)
{
        if (get_determinant(symOp)==1 && get_number_of_eigenvalues_of_one(symOp)==1)// && is_tau_lattice_translation==true
	{
		return true;
	}
        else
		return false;
}

bool is_symop_improper_rotation(Eigen::Matrix3f symOp)//may combine improper rot and mirror plane
{
	if (get_determinant(symOp)==-1 && get_number_of_eigenvalues_of_one(symOp)==1)
	{
		return true;
	}
        else
		return false;
}

bool is_symop_mirror_plane(Eigen::Matrix3f symOp)
{
        if (get_determinant(symOp)==-1 && get_number_of_eigenvalues_of_one(symOp)==2)// && is_tau_lattice_translation==true
	{
		return true;
	}
        else
		return false;
}

bool is_symop_inversion(Eigen::Matrix3f symOp)
{
	if (get_trace==-3 && get_determinant(symOp)==-1 && get_number_of_eigenvalues_of_one(symOp)==0)
	{
		return true;
	}
        else
		return false;
}

//bool is_symop_screw_axis(Eigen::Matrix3f symOp, double tau);
//{
//        if get_determinant(symOp)==1 && get_number_of_eigenvalues_of_one(symOp)==1 && is_tau_lattice_translation==false
//	{
//		return true;
//	}
//        else
//		return false;
//}
//
//bool is_symop_glide_plane(Eigen::Matrix3f symOp, double tau);
//{
//        if get_determinant(symOp)==-1 && get_number_of_eigenvalues_of_one(symOp)==2 && is_tau_lattice_translation==false
//	{
//		return true;
//	}
//        else
//		return false;
//}


int main()
{

	const auto lattice=Get_Eigen_lattice(filename);
	auto validsymops= Calculate_point_group(lattice, 1);
    std::vector<Eigen::Vector3f> high_symmetry_points; //just coords stored together
    std::vector<Eigen::Vector3f> high_symmetry_axes; //points and vector stored together?
    std::vector<Eigen::Matrix<float, 2, 3>> high_symmetry_planes;//
        for (auto symOps : validsymops)
	{
		if (is_symop_identity(symOps)==true)
		{
                //do nothing? have the basis as the high symmetry points?
		}
		else if (is_symop_rotation(symOps)==true)
		{
                high_symmetry_axes.push_back(symOps.eigenvectors());
		//Find vector of rotation axis, store all kpoints along line as high sym points?
		}
		else if (is_symop_improper_rotation(symOps)==true)
		{
	        high_symmetry_axes.push_back(symOps.eigenvectors());		
                //Find vector of rotation axis, store all kpoints along line as high sym points?
		}
		else if (is_symop_mirror_plane(symOps)==true)		
		{
		high_symmetry_planes.push_back(symOps.eigenvectors());
		//Find vectors of plane, store all kpoints along line as high sym points?
		}
		else if (is_symop_inversion(symOps)==true)
		{
		high_symmetry_planes.pushback(symOps.eigenvectors());
		//Find vectors of plane, store all kpoints along line as high sym points?
		}
	//	else if is_symop_screw_axis(symOps)==true
	//	{
	//	high_symmetry_axes.pushback(symOps.eigenvectors());
	//	//Find vector of axis, store all kpoints along line as high sym points?
	//	}
	//	else if is_symop_glide_plane(symOps)==true
	//	{
	//	high_symmetry_planes.pushback(symOps.eigenvectors());
	//	//Find vectors of plane, store all kpoints along line as high sym points?
	//        }
	}	
	//print out all high sym points/axes and planes?	
	//need check for if there are any symops that are considered both. If so, something must check for this and let us know as this means we have an error in our midsts
}
