#include "eigen-git-mirror/Eigen/Dense"
#include "iostream"
#include "fstream"
#include "list"
#include "iomanip"
//***Creating grid points
//std::vector<Rloat> CreateGridPoints(double l, int r){
//**need to get a & b from l
//#include "vector"

typedef Eigen::Matrix<double,2,2> Matrix2f;
bool is_sym_op(const Matrix2f O){
    if (O.determinant()!=1 || O.determinant()!=-1){
        return false;
    }
    if (O.transpose()*O != Matrix2f::Identity()){
        return false;
    }
    else return true;
}





int main(void)
{
    int x,y;
typedef Eigen::Matrix<double, 2, 2> Matrix2f;
Matrix2f l;
//double l[2][2];
    std::ifstream input;
    input.open("input.txt", std::ios::in);

    if (input.is_open()){
    for (x=0; x < 2; x++){
        for (y=0; y < 2; y++){
            input >> l(x,y) ;
 //           std::cout << l(x,y) << ' ';
        }
  //      std::cout << std::endl;
    }
    input.close();
    }

    else std::cout << "Unable to open file\n";

std::cout << l << std::endl;
//Calculation of the grid points
//double a[2], b[2];
typedef Eigen::Matrix<double,2,1> Vector2f;
Vector2f a,b;
int r=5;
int i,j;

//a[0]=l[0][0];
//a[1]=l[0][1];

//b[0]=l[1][0];
//b[1]=l[1][1];
//std::cout << l(0,1) << std::endl;
a << l(0,0), l(1,0);
b << l(0,1), l(1,1);
std::cout << a << std::endl;
std::cout << b << std::endl;

std::list<Vector2f> p;
    for (i=-r; i<=r; i++){
        for (j=-r; j<=r; j++){
            p.push_back({i*a(0,0)+j*b(0,0), i*a(1,0)+j*b(1,0)});
        }
    }
//calculating the list of lprimes
Matrix2f m;

std::list<Matrix2f> lprime;

for (auto &v : p)
{
    for (auto &w : p){
        m(0,0) = v(0,0);
        m(0,1) = w(0,0);
        m(1,0) = v(1,0);
        m(1,1) = w(1,0);
//        std::cout << m << std::endl;
        lprime.push_back(m);
    }
}
std::cout << lprime.size() << std::endl;

//for (auto &x : lprime){
  //              std::cout << x << std::endl;
  //              std::cout << std::endl;
   //       std::cout << "**************" << std::endl;
//}
//}

//calculating the list of possible symmetry ops
std::list<Matrix2f> Ss;
Matrix2f S,t;
//Matrix2f test;

//test <<  0.5, -0.8660254,
// 0.8660254, 0.5;
//std::cout << test.transpose()*test << std::endl;

for (auto &z: lprime){
S = z*l.inverse();


if (S.determinant()==1 || S.determinant()==-1){
 //if (t.isIdentity(1e-3)==1){
    //std::cout << S.transpose()*S << std::endl;
    //std::cout << "*************" << std::endl;
    //std::cout << S.transpose()*S << std::endl;
   t = S.transpose()*S;
   int check=t.isIdentity(1e-4);
    //std::cout << t << std::endl;
    //std::cout << "------------------" << std::endl;
    //std::cout << t.isIdentity(1e-4) << std::endl;
   if (t.isIdentity(1e-4)==1){
    std::cout << S << std::endl;
    std::cout << "**********************" << std::endl;
   }
      //    std::cout << S << std::endl;
  // std::cout << "***********" << std::endl;
  //  std::cout << S.determinant()<<std::endl;

//}
    //    std::cout << S.determinant()<< std::endl;
}
//std::cout << S << std::endl;//
//if (is_sym_op(S)==true){
  //  Ss.push_back(S);
//}
}
}






