#include <iostream>
#include <math.h>
#include <vector>
#include "eigen-git-mirror/Eigen/Dense"
#include "eigen-git-mirror/Eigen/LU"
#include <fstream>

using namespace std;


bool is_S_Valid(Eigen::MatrixXd S, double delta=0.001){
Eigen::MatrixXd SMultST(2,2);
double SDet;
SMultST=S.transpose()*S;
SDet=S.determinant();
if(fabs(1-fabs(SDet))<delta&&((fabs(1-SMultST(0,0))<delta)&&(fabs(1-SMultST(1,1))<delta)&&(fabs(SMultST(0,1))<delta)))
  return true;
else
return false;
}

vector<vector<double>> AllPoints(Eigen::MatrixXd lattice, int radius)
{
vector<double> tempV{0,0};
vector<vector<double>> points;
     for (int m=-radius;m<radius+1;m=m+1)
for (int n=-radius;n<radius+1;n=n+1)
{
tempV[0]=double(m*lattice(0,0)+n*lattice(0,1));
tempV[1]=double(m*lattice(1,0)+n*lattice(1,1));
points.push_back(tempV);
}
     return points;
}

Eigen::MatrixXd ReadingMatrixFromFile(const char * filename)
{

Eigen::MatrixXd lattice(2,2);
ifstream fp(filename);
if (! fp) {
cout << "Error, file couldn't be opened" << endl;
}
for(int row = 0; row < 2; row++) {  // stop loops if nothing to read
for(int column = 0; column < 2; column++){
fp >> lattice(row,column);
if ( ! fp ) {
cout << "Error reading file for element " << row << "," << column << endl;
}
}
}
return lattice;
}


int main()
{
Eigen::MatrixXd lattice(2,2);
Eigen::MatrixXd latticePrime(2,2);
Eigen::MatrixXd S(2,2);

char FilePath[200]="/home/farnaz/CASM/JohnG/casm-utilities/avdv_pgroup_farnaz/avdv-point-group/Lattice.txt";

lattice=ReadingMatrixFromFile(FilePath);

//lattice<<1, -0.5, 0, 0.86602540378;

cout<<lattice<<endl<<endl;

double delta=0.001;
int radius=4;
vector<vector<double>> points=AllPoints(lattice, radius);
int numberOfValidS=0;
/////////////////////////////////////from here//////////////
for (int i=0; i<points.size();i=i+1)
for (int j=0; j<points.size();j=j+1)
{
latticePrime(0,0)=points[i][0];
latticePrime(1,0)=points[i][1];
latticePrime(0,1)=points[j][0];
latticePrime(1,1)=points[j][1];

S=latticePrime*lattice.inverse();
if(is_S_Valid(S,delta))
{
cout<<S<<endl<<endl;
cout<<"######################################################"<<endl;
numberOfValidS=numberOfValidS+1;
}
/////////////////////to here it can also be a function, do we need to write S to file?
  }
cout<<"Total Number of Valid S:  "<<numberOfValidS<<endl;


return 0;
}
