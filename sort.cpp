#include "../eigen-git-mirror/Eigen/Dense"
#include "../eigen-git-mirror/Eigen/Eigenvalues"
#include <iostream>
#include <vector>
#include <cmath>

typedef Eigen::Matrix<double,3,1> Vector3f;
typedef Eigen::Matrix<double,3,3> Matrix3f;
typedef Eigen::EigenSolver<Matrix3f> eigensolver;

#define PREC 1e-6 //Precision for double comparisions

//creating a symmetry operations class, with a rotation part and a translation part
class sym_ops{
    public:
        Matrix3f rotation;
        Vector3f translation;
        std::string label;
    //Creating a constructor to define the rotation and the translation part
        sym_ops(Matrix3f r, Vector3f tau){
            rotation = r;
            translation = tau;
        }
};

//Function to compare doubles
bool ae(double a, double b, double absEpsilon, double relEpsilon){
    // Check if the numbers are really close -- needed when comparing numbers near zero.
    double diff{ fabs(a - b)  };
    if (diff <= absEpsilon)
        return true;
    // Otherwise fall back to Knuth's algorithm
    return diff <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * relEpsilon );
//
}

//Function to check if the eigen values are equal to one
int check_eigen_values(sym_ops sym_op_in){
    
    eigensolver ev(sym_op_in.rotation, false);
    Vector3f evs;
    evs = ev.eigenvalues().real();
    
    int counter=0;
    for (int i=0; i<3; i++){
       if (evs(i,0)==1){
           counter = counter+1;
        }
    }
    return counter;
}

//Function to check if the translation part of sym op is a lattice translation
bool is_tau_lattice_translation(sym_ops sym_op_in, Matrix3f lattice){
    
   //Defining all zeros case to avoid any divisions by zero 
    if (ae(sym_op_in.translation(0,0),0,PREC,PREC) == true && ae(sym_op_in.translation(1,0),0,PREC,PREC) == true && ae(sym_op_in.translation(2,0),0,PREC,PREC) == true){
        return true;
    }
    
    //compute the coefficients of the linear combination of lattice vectors
    Vector3f check_matrix;
    check_matrix = lattice.inverse()*sym_op_in.translation; 
  
    //Check if those coefficients are integers 
    if (ae(check_matrix(0,0)-(int)(check_matrix(0,0)),0,PREC,PREC) == true && ae(check_matrix(1,0)-(int)(check_matrix(1,0)),0,PREC,PREC) == true && ae(check_matrix(2,0)-(int)check_matrix(2,0),0,PREC,PREC) == true){
        return true;
    }
    return false;
}

//Generate labels for various sym ops based on the conditions
std::vector<sym_ops> label_sym_ops(std::vector<sym_ops> &sym_op_in, Matrix3f lattice){
     
    for (auto &iterator: sym_op_in){
    
        if (ae(iterator.rotation.determinant(),1,PREC,PREC) == true && ae(iterator.rotation.trace(),3,PREC,PREC) == true && is_tau_lattice_translation(iterator,lattice)==true){
        iterator.label = "Identity";    
        }
        
        else if (ae(iterator.rotation.determinant(),-1,PREC,PREC) == true && ae(iterator.rotation.trace(),-3,PREC,PREC) == true && is_tau_lattice_translation(iterator,lattice)==true){
        iterator.label = "Inverse";
        }
        
        else if (ae(iterator.rotation.determinant(),1,PREC,PREC) == true && check_eigen_values(iterator)==1 && is_tau_lattice_translation(iterator,lattice) == true){
        iterator.label = "Rotation";
        }
        
        else if (ae(iterator.rotation.determinant(),-1,PREC,PREC) == true && check_eigen_values(iterator)==1 && is_tau_lattice_translation(iterator,lattice) == true){
        iterator.label = "Improper Rotation";
        }
        
        else if (ae(iterator.rotation.determinant(),-1,PREC,PREC) == true && check_eigen_values(iterator)==2 && is_tau_lattice_translation(iterator, lattice) == true){
        iterator.label = "Mirror";
        }
        
        else if (ae(iterator.rotation.determinant(),1,PREC,PREC) == true && check_eigen_values(iterator)==1 && is_tau_lattice_translation(iterator, lattice) == false){
        iterator.label = "Screw";
        }
         
        else if (ae(iterator.rotation.determinant(),-1,PREC,PREC) == true && check_eigen_values(iterator)==2 && is_tau_lattice_translation(iterator, lattice) == false){
        iterator.label = "Glide";
        }

        else iterator.label = "Yoo!! Not a sym_op dude";
    }
    return sym_op_in;
}

int main(int argc, char *argv[]){
   //checking by proving some random sym ops 
    Matrix3f t1,t3,l;
    Vector3f t2,t4;

    l << 1,0,0,
      0,1,0,
      0,1,2;

    t1 << 1,0,0,
       0,1,0,
       0,0,-1;
    t2 << 2,0,0;
  
    t3 << 1,0,0,
       0,1,0,
       0,0,-1;
    t4 << 1,0,7;
    std::vector<sym_ops> test,out;
    
    sym_ops x1(t1,t2);
    sym_ops x2(t3,t4);
    
    test.push_back(x1);
    test.push_back(x2);
  
    out = label_sym_ops(test,l);
    
    for (auto &x: out){
        std::cout << "----------------------" << std::endl;
        std::cout << x.label << std::endl;
        std::cout << "----------------------" << std::endl;
        std::cout << "The rotational part is" << std::endl;
        std::cout << x.rotation << std::endl;
        std::cout << "The translation part is" << std::endl;
        std::cout << x.translation << std::endl;
    } 
}

        
