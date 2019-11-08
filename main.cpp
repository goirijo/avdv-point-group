#include "Eigen/Dense"
#include <iostream>
#include <cmath>
#include <fstream>
#include "io_func.cpp"
#include <vector>

using namespace std;

bool is_valid(const Eigen::Matrix2d S)
{//check if SymOp is Unitary by checking if transpose==inverse and det is +/-1
//    Eigen::MatrixXd I = Eigen::MatrixXd::Identity(3,3);
    Eigen::Matrix2d sst=S.transpose()*S;

    if (sst.isIdentity()==false){ return false;}
    auto det=S.determinant();
    if((1-abs(det))<.00001){return true;}
    else {return false;}
}

auto create_grid_pts(const Eigen::Matrix2d L)
{
//generate a grid of coordinates of points with radius n.
//each coordinate is vector pair of doubles in a 2n+1 by 2n+1 eigen matrix
        double n=5.0;
        int  l=2*n+1;
	std::vector< std::vector<double>> grid;
	for(int i=0; i<l;i++){
		for(int j=0; j<l; j++){
			std::vector<double> temp;
			double ii=i;
			double jj=j;
			temp.push_back((ii-n)*L(0,0)+ (jj-n)*L(0,1));
			temp.push_back((ii-n)*L(1,0)+ (jj-n)*L(1,1));
			grid.push_back(temp);
//			if(j==0 &&i==5){std::cout<<temp[0] << "  "<<temp[1]<<endl;}
			}
	}

	return grid;
}

auto calc_L_primes(const std::vector< std::vector<double>> grid)
{
//calculate the L-primes, which are two pairs of coordinates as lattice vectors
//returns
	std::vector<Eigen::Matrix2d> L_prime;
//	std::vector<double> p1, 2;
        int ct=0;
	for(const auto p1 : grid){
	    for( const auto p2 : grid){
			Eigen::Matrix2d temp;
			temp(0,0)=p1[0];
			temp(1,0)=p1[1];
			temp(0,1)=p2[0];
			temp(1,1)=p2[1];
//			if(ct<5){
//				std::cout<<temp<<endl;
//				ct++;
//			}
			L_prime.push_back(temp);
	    }
	}
	return L_prime;
}

auto calc_point_group(const Eigen::Matrix2d L)
{  
//calculate all valid SymOps for the input Lattice
//returns vector of SymOp matrices
//
   	std::vector<Eigen::Matrix2d> SymOps;
	auto pts = create_grid_pts(L);
	auto L_primes = calc_L_primes(pts);
	for(Eigen::Matrix2d LP:L_primes){
		Eigen::Matrix2d temp;
		temp=LP*L.inverse();
		if(is_valid(temp)){
			SymOps.push_back(temp);
		}
	}
	return SymOps;
}

int main(int argc, char *argv[])
{ 
//main function which reads in lattice from input file and calcualted point group
//and prints it to the screen

    Eigen::Matrix2d L = read_lattice(argv[1]);
    std::cout<<"This is the Lattice:"<<endl;
    std::cout<<L<<endl;
    std::vector<Eigen::Matrix2d> pt_grp;

    pt_grp = calc_point_group(L);
//    std::cout<<pt_grp.size()<<endl;
    print_ptgroup(pt_grp);
    return 0;
}
