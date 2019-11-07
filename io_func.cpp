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
     Eigen::Matrix2f  L;
     fstream file;
     std::string word;
     file.open(filename.c_str());
     int count=0;
     int i=0;
     while(file >> word) {
           int j=count%2;
           if(count==2){i++;}
           L(i,j)=std::stof(word);
     }
     return L;
}


void print_ptgroup(const std::vector<Eigen::Matrix2f> group)
{
//Prints the number of symmops and then lists each of them
      int numops=size(group);
      std::cout<< "Point Group has " numops "Symmetry Operations"<<endl;

      int count=1;
      for(Eigen::Matrix2f op:group){
          str::cout<<"Symmetry Op " count<<endl;
          str::cout<<op<<endl;
          count++;
      }
      return 0;
}

