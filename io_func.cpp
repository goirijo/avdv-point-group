#include "Eigen/Dense"
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include<vector>
#include <set>

using namespace std;

auto read_lattice(string filename)
{
//read file and take each word and use it to populate L, an Eigen matrix
     Eigen::Matrix2d  L;
     fstream file;
     std::string word;
     file.open(filename.c_str());
     int count=0;
     int i=0;
     while(file >> word) {
           int j=count%2;
           if(count==2){i++;}
	   L(i,j)=std::stod(word);
	   count++;
     }
     return L;
}


void print_ptgroup(const std::vector<Eigen::Matrix2d> group)
{
//Prints the number of symmops and then lists each of them
      int numops=group.size();
      std::cout<< "Point Group has " << numops << "Symmetry Operations"<<endl;

      int count=1;
      for(Eigen::Matrix2d op:group){
          std::cout<<"Symmetry Op " <<count<<endl;
          std::cout<<op<<endl;
          count++;
      }
      return;
}

