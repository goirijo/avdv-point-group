#include "eigen-git-mirror/Eigen/Dense"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <ostream>

using namespace Eigen;
 //Initial Class set up 

    std::vector<std::vector<double>> calculate_gridpoints(std::vector<std::vector<double>> lattice, int radius)	 //Might take in a structure instead and get a lattice from it
    {
	  std::vector<std::vector<double>> gridpoints; //left in int since I put radius in int
	  std::vector<int> pn;
	  std::vector<int> pm;
	  for (int m=-radius; m<(radius+1); m++) 
   	  {
		 for (int n=-radius; n<(radius+1); n++)
		 {
	          gridpoints.emplace(
                 gridpoints.emplace(n * lattice[0][0]);
		 gridpoints.emplace(n * lattice[1][0]);  
       	         }
             	
              gridpoints.emplace(m * lattice[0][1]);
              gridpoints.emplace(m * lattice[1][1]);
    	  }
   	 return gridpoints;
    }

     std::vector<std::vector<double>> Calculate_Lprime(std::vector<std::vector<double>> lattice, int radius)  //Again may need to go with Structure
     {
            std::vector<std::vector<double>> Lprimes;
            std::vector<std::vector<double>> PS=calculate_gridpoints(lattice, radius)
            for (auto p1 : ps)
            {
        	    for (auto p2: ps)
        	    {
        		   Lprimes.push_back(p1,p2)
        	    }
            }
            return Lprimes;  
     }
 
     Calculate_point_group(std::vector<std::vector<double>> lattice, int radius) //Is the type symops?
     {  
           std::vector<std::vector<double>> validsymops;
           std::vector<std::vector<double>> PS=calc_grid_points(lattice, radius)
           std::vector<std::vector<double>> Lprimes=Calculate_Lprime(lattice, radius);
           for (auto Lp : Lprimes)
           {
        	   SymmetryOp=Lprimes*Lattice;
                        if (is_symop_valid(SymmetryOp))
        	         	validsymops.push_back();
           }
      return validsymops;
     }
            
 
//    bool is_SymOp_Unitary(SymOp SymMatrix); //Look up C++ general determinant calcs
 
    bool is_symop_valid(std::vector<std::vector<double>> SymMatrix);
    { 
            if (SymMatrix.determinant()>-1.11&& SymMatrix.determinant()<-0.99) ||(SymMatrix.determinant()>0.99&&SymMatrix.determinant()<1.01)
        	    return false;
            else if SymOp*Transpose(Symop)!={{1,0,0},{0,1,0},{0, 0, 1}}
                    return false;
        return true;
    }
 
 
   // bool is_group_closed(SymOp SymMatrix); //
 
 
   // void print_mult_table(SymOp SymMatrix)    
    

int main()
{
	double lattice[2][2]={{2.5, 0.2}, {0.3, 2.1}}
        return calculate_gridpoints(lattice, 5);
  //   std::vector<std::vector<double>> lattice[2.5, 0.3][0.2, 2.1]
//   std::ifstream in("input_lattices/ugly.txt");
//   std::vector<std::vector<double>> lattice;
//   if (!in.is_open()) {
//        std::cerr << "There was a problem opening the input file!\n";
//        exit(1);//exit or do additional error checking
 //  }

 //  if (in) {
 //       std::string line;
 //       int value;
 //       while (std::getline(in, line)) {
 //           lattice.push_back(std::vector<double>());
//	    std::cout << "this is lattice step 1:" << line << '\n';  
 //             for (int i = 0; i < 3; i++) {
 //                 for (int j = 0; j < 3; j++)
  //                  fscanf(in, "%lf",&value[i][j]);
//		}
 //       }
 //   }

//    for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 3; j++)
//            std::cout << lattice[i][j] << ' ';
//
 //       std::cout << '\n';
 //   }
 
   return 0;
}
