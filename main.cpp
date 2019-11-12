#include "eigen-git-mirror/Eigen/Dense"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <ostream>

//Initial Class set up 
//Calculate the grid from which we will calculate potential Lprime values. Need 3 by 3 integers 
std::vector<Eigen::Vector3f> calculate_gridpoints(Eigen::Matrix3f lattice, int radius)	 //Might take in a structure instead and get a lattice from it
{
	std::vector<Eigen::Vector3f> gridpoints; //left in int since I put radius in int
	Eigen::Vector3f pn;
	for (int m=-radius; m<(radius+1); m++) 
	{
		for (int n=-radius; n<(radius+1); n++)

		{
			for (int j=-radius; j<(radius+1); j++)
			{
				pn=n*lattice.col(0)+m*lattice.col(1)+j*lattice.col(2);
				gridpoints.push_back(pn);	
		
			}
                }
	}

	return gridpoints;
}
//change complete


//calculate a list of potentials Lprimes
std::vector<Eigen::Matrix3f> Calculate_Lprimes(Eigen::Matrix3f lattice, int radius)  //Again may need to go with Structure
{
	std::vector<Eigen::Matrix3f> Lprimes;
	auto PS=calculate_gridpoints(lattice, radius);
	Eigen::Matrix3f MakeMatrix;
	for (auto p1 : PS)
	{
		for (auto p2 : PS)
		{
			for (auto p3 : PS)
			{
				MakeMatrix<<p1(0), p2(0), p3(0), 
				        	p1(1), p2(1), p3(1),
				        	p1(2), p2(2), p3(2);  //I may have these flipped. If Im supposed to have [p1][p2] then this woulr be correct
				Lprimes.push_back(MakeMatrix.inverse());
                        }
		}
	}
	return Lprimes;  
}
//change complete

//Determine whether or not the calculated Symmetry operation is valid through comparison of S^T*S=I
bool is_symop_valid(Eigen::Matrix3f SymMatrix)
{
	auto Matrixcheck= SymMatrix.transpose()*SymMatrix;
	if (!Matrixcheck.isIdentity(.0005))
		return false;
	else 
		return true; 
}
//This function calculates the symmetry operations that are valid for a given lattice
std::vector<Eigen::Matrix3f> Calculate_point_group(Eigen::Matrix3f lattice, int radius) //Is the type symops?
{  
	std::vector<Eigen::Matrix3f> validsymops;
	auto Lprimes=Calculate_Lprimes(lattice, radius);
	Eigen::Matrix3f SymmetryOp;
	for (auto Lp : Lprimes)
	{
		SymmetryOp=lattice*Lp;
		if (is_symop_valid(SymmetryOp))
		{
			validsymops.push_back(SymmetryOp);
		}
	}
	return validsymops;
}



bool MatrixComparison(Eigen::Matrix3f Matrix1, Eigen::Matrix3f Matrix2)
{
	return Matrix1.isApprox(Matrix2);
}



//    bool is_SymOp_Unitary(SymOp SymMatrix); //Look up C++ general determinant calcs

struct compare_mat{
	compare_mat(Eigen::Matrix3f Matrix1) : Matrix1(Matrix1) {}
	bool operator()(Eigen::Matrix3f Matrix2) const {
		return MatrixComparison(Matrix1, Matrix2);
	}

	private:
	Eigen::Matrix3f Matrix1;
};


//
bool group_is_closed(std::vector<Eigen::Matrix3f> SymMatrix) //
{
	Eigen::Matrix3f GroupMultiplication;
	for (auto S1: SymMatrix)
	{
		for(auto S2:SymMatrix)
		{
			GroupMultiplication=S1*S2;
			compare_mat compare_matrix(GroupMultiplication);

			if (std::find_if(SymMatrix.begin(), SymMatrix.end(), compare_matrix) != SymMatrix.end())
				return true;
			else      
				return false;
		}
	}

}


//
//  // void print_mult_table(SymOp SymMatrix)    

//get lattice as vector of vectors from text file
std::vector<std::vector<double>> get_lattice(std::string filename)
{
	std::ifstream in(filename);
	std::vector<std::vector<double>> lattice; 
	if (!in.is_open()) 
	{
		std::cerr << "There was a problem opening the input file!\n";
		exit(1);//exit or do additional error checking
	}  
	std::string line;
	double value;
	while (std::getline(in, line)) {
		std::istringstream iss(line);
		std::vector<double> templattice;
		while (iss>>value)
		{ 
			templattice.push_back(value);
		}
		lattice.push_back(templattice);
	}   
	return lattice; 	

}

//convert vector lattice to eigen lattice
Eigen::Matrix3f Get_Eigen_lattice(std::string filename)
{ 
	std::vector<std::vector<double>> lat=get_lattice(filename);
	Eigen::Matrix3f m;
	m<<lat[0][0], lat[0][1], lat[0][2],
	lat[1][0], lat[1][1], lat[1][2],
	lat[2][0], lat[2][1], lat[2][2]; 
	return m;
}

int main(int argc, char *argv[])
{
	if (argc<2)
	{
		std::cout<<"You;ve made a terrible mistake. Enter text file."<<'\n';
	        return 1;
	}
	std::string filename=argv[1];

	const auto lattice=Get_Eigen_lattice(filename);
	auto gridpoints=calculate_gridpoints(lattice,1);
	std::cout<<"The lattice we are considering is"<<'\n'<<lattice<<'\n'<<'\n';     	   
	auto Lprimes=Calculate_Lprimes(lattice, 1);
		
	auto validsymops= Calculate_point_group(lattice, 1);  
	std::cout<<"The valid symmetry operations are:"<<'\n'; 
	for (int i=0; i<validsymops.size();i++)
	 	std::cout<<validsymops[i] <<'\n'<<'\n';	
                  
	if (group_is_closed(validsymops))
		std::cout<<"The group is closed!"<<'\n';
	else
		std::cout<<"The group is not closed!"<<'\n';
	std::cout<<"These are the valid symops"<<'\n'<< validsymops.size()<<'\n';

	return 0;
}
