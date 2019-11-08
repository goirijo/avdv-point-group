#include "eigen-git-mirror/Eigen/Dense"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <ostream>

 //Initial Class set up 
 //Calculate the grid from which we will calculate potential Lprime values
 std::vector<Eigen::Vector2f> calculate_gridpoints(Eigen::Matrix2f lattice, int radius)	 //Might take in a structure instead and get a lattice from it
 {
       std::vector<Eigen::Vector2f> gridpoints; //left in int since I put radius in int
       Eigen::Vector2f pn;
       for (int m=-radius; m<(radius+1); m++) 
	   {
     	    for (int n=-radius; n<(radius+1); n++)
     	        {
	         pn=n*lattice.col(0)+m*lattice.col(1);
            
                  gridpoints.push_back(pn);	
    	         }

 	  }
        
	 return gridpoints;
 }
  //calculate a list of potentials Lprimes
   std::vector<Eigen::Matrix2f> Calculate_Lprimes(Eigen::Matrix2f lattice, int radius)  //Again may need to go with Structure
   {
	  std::vector<Eigen::Matrix2f> Lprimes;
          auto PS=calculate_gridpoints(lattice, radius);
	  Eigen::Matrix2f MakeMatrix;
          for (auto p1 : PS)
            {
      	       for (auto p2: PS)
      	       {
      		   MakeMatrix<<p1(0), p2(0), 
			     p1(1), p2(1);  //I may have these flipped. If Im supposed to have [p1][p2] then this woulr be correct
		   Lprimes.push_back(MakeMatrix.inverse());
       	 
              }
          }
          return Lprimes;  
   }
   
 //Determine whether or not the calculated Symmetry operation is valid through comparison of S^T*S=I
   bool is_symop_valid(Eigen::Matrix2f SymMatrix)
   {
	  auto Matrixcheck= SymMatrix.transpose()*SymMatrix;
          if (!Matrixcheck.isIdentity(.05))
		 return false;
	  else 
		 return true; 
   }
   //This function calculates the symmetry operations that are valid for a given lattice
   std::vector<Eigen::Matrix2f> Calculate_point_group(Eigen::Matrix2f lattice, int radius) //Is the type symops?
   {  
         std::vector<Eigen::Matrix2f> validsymops;
         auto Lprimes=Calculate_Lprimes(lattice, radius);
         Eigen::Matrix2f SymmetryOp;
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
          

//    bool is_SymOp_Unitary(SymOp SymMatrix); //Look up C++ general determinant calcs

 

 //
 //  // bool is_group_closed(SymOp SymMatrix); //
 //
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
   Eigen::Matrix2f Get_Eigen_lattice(std::string filename)
   { 
	  std::vector<std::vector<double>> lat=get_lattice(filename);
          Eigen::Matrix2f m;
	  m<<lat[0][0], lat[0][1],
	     lat[1][0], lat[1][1]; 
     return m;
   }

int main()
{
    auto lattice=Get_Eigen_lattice("input_lattices/ugly.txt");
    auto gridpoints=calculate_gridpoints(lattice,5);
    std::cout<<"The lattice we are considering is"<<'\n'<<lattice<<'\n'<<'\n';     	   
    auto Lprimes=Calculate_Lprimes(lattice, 5);
    auto validsymops= Calculate_point_group(lattice, 5);  
    std::cout<<"The valid symmetry operations are:"<<'\n'; 
    for (int i=0; i<validsymops.size();i++)
       std::cout<<validsymops[i] <<'\n'<<'\n';	
   return 0;
}
