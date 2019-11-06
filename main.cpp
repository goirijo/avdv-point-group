#include "eigen-git-mirror/Eigen/Dense"
#include <iostream>
#include <fstream>
#include <list>
#include "src/headers.h"

int main(int argc, char *argv[])
{
//*******Reading the lattice**********************
    int x,y;
    Matrix2f l;

    std::ifstream input;
    input.open(argv[1], std::ios::in);

    if (input.is_open()){
    for (x=0; x < 2; x++){
        for (y=0; y < 2; y++){
            input >> l(x,y) ;
        }
    }
    input.close();
    }

    else std::cout << "Unable to open file\n";
//**************************************************
//********Calculating grid points*******************
    std::list<Vector2f> pr;
    pr = calc_grid_points(5,l);
//**************************************************
//*******Calculating the list of lprimes************
    std::list<Matrix2f> lprimes;
    lprimes=calc_l_primes(pr);

//**************************************************
//*****Calculating the list of possible symmetry ops
    std::list<Matrix2f> Ss;
    Ss=calc_point_group(lprimes,l);

//***********Printing out the symmetry ops**********
    std::cout << "***********************" << std::endl;
    std::cout << "The number of operations in the group: " << Ss.size() << std::endl;
    std::cout << "***********************" << std::endl;
    for (auto &symops: Ss){
        std::cout << symops << std::endl;
        std::cout << "***********************"<<std::endl;
    }
    return 0;
}






