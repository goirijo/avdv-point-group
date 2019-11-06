//#include "Eigen/Dense"
#include <iostream>
#include <math.h>


//making list of point groups. let's call them S. first check if they are valid or not: 
using namespace std;
bool S_is_Valid(double tmp_s[2][2]);

bool S_is_Valid(double tmp_s[2][2]){
double tmp_s_det;  
double StMulS[2][2];
double delta=0.0001;

tmp_s_det=(tmp_s[0][0]*tmp_s[1][1]-tmp_s[0][1]*tmp_s[1][0]);
            // calculating S^T*S
            StMulS[0][0]=tmp_s[0][0]*tmp_s[0][0]+tmp_s[1][0]*tmp_s[1][0];
            StMulS[0][1]=tmp_s[0][0]*tmp_s[0][1]+tmp_s[1][0]*tmp_s[1][1];
            StMulS[1][0]=tmp_s[0][0]*tmp_s[0][1]+tmp_s[1][0]*tmp_s[1][1];
            StMulS[1][1]=tmp_s[0][1]*tmp_s[0][1]+tmp_s[1][1]*tmp_s[1][1];
           
            //now checking if the generated S satisfies two conditions : (i) det(S)=+-1, and S^T x S=I
            //NOTEEEE: this needs to be modified based on Eign functions since now the values might be 0.9999999 rather than 1 for triangular lattice .
            //if(((1-tmp_s_det<delta)||(1-tmp_s_det>-delta))&&((StMulS[0][0]==1)&&(StMulS[0][1]==0)&&(StMulS[1][1]==1)))
            if(  fabs(1-fabs(tmp_s_det))<delta  &&  (   (fabs(1- StMulS[0][0])<delta) && (fabs(1- StMulS[1][1])<delta)  && (fabs(StMulS[0][1])<delta)   )  )
                return true;
            else
                return false;
}

int main()
{
    double lattice[2][2]={{2,0},{0,2}};
    //double lattice[2][2]={{1,-0.5},{0,0.86602540378}};
   
    //just printing the given lattice
    for (int i=0 ;i<2; i=i+1)
      for (int j=0;j<2;j=j+1)
        cout<<lattice[i][j]<<endl;
   
   //making grid and list of points based on radius
   int radius=4;
   //since I want the lattice m*n rows and 2 column I can write it based on radius, the first element is x, the second is y of the point.
   double point[((2*radius)+1)*((2*radius)+1)][2];
   int p_row=0;
   for (int m=-radius;m<radius+1;m=m+1) {
        for (int n=-radius;n<radius+1;n=n+1){
            //each new wector is a linear combination of the given two vectors: m*a+n*b, so  point(x) => point[i][0]=ma1+nb1  and point(y)=> point[i][1]=ma2+nb2 (a1 = x of point 1 in lattice,a2 y of point 1 in lattice ,..)
            // here fiiling the x & y of the points
            point[p_row][0]=m*lattice[0][0]+n*lattice[0][1];
            point[p_row][1]=m*lattice[1][0]+n*lattice[1][1];
            p_row=p_row+1;
        }
    }

    double S[400][4];
    int S_row=0;
    double L_prim[2][2];
    double tmp_s[2][2];
    // S transpose multiply by S
    double StMulS[2][2];

    double L_det,tmp_s_det;
    int S_index=0;
    //calculating the determinan of the given lattice matrix
    L_det=(lattice[0][0]*lattice[1][1]-lattice[0][1]*lattice[1][0]);
    // in these nested for loops, we first creat the L', then calculate the S function which is S=L'xL^(-1),
    //after calculating S, we need to check whether it satisfy the conditions. So we first calculate the determinan of S and then S^TxS
    for (int i=0;i<p_row;i=i+1){
        for (int j=0;j<p_row;j=j+1){
            //creating L'
            L_prim[0][0]=point[i][0];
            L_prim[1][0]=point[i][1];
            L_prim[0][1]=point[j][0];
            L_prim[1][1]=point[j][1];
            //Calculating S=L'*L^(-1)
            tmp_s[0][0]=(1/L_det)*(L_prim[0][0]*lattice[1][1]-L_prim[0][1]*lattice[1][0]);
            tmp_s[0][1]=(1/L_det)*(L_prim[0][1]*lattice[0][0]-L_prim[0][0]*lattice[0][1]);
            tmp_s[1][0]=(1/L_det)*(L_prim[1][0]*lattice[1][1]-L_prim[1][1]*lattice[1][0]);
            tmp_s[1][1]=(1/L_det)*(L_prim[1][1]*lattice[0][0]-L_prim[1][0]*lattice[0][1]);

            //now checking if the generated S satisfies two conditions : (i) det(S)=+-1, and S^T x S=I
            //NOTEEEE: this needs to be modified based on Eign functions since now the values might be 0.9999999 rather than 1.
            if(S_is_Valid(tmp_s))
                {
                    //if S satisfies these conditions, we store them in the S matrix
                    S[S_index][0]=tmp_s[0][0];
                    S[S_index][1]=tmp_s[0][1];
                    S[S_index][2]=tmp_s[1][0];
                    S[S_index][3]=tmp_s[1][1];
                    S_index=S_index+1;
                }
               

        }
    }
   
   //let's print the stored S!
   cout<<endl<<endl;
for (int i=0; i<S_index;i=i+1)  {
    cout<<S[i][0]<<"  "<<S[i][1]<<endl;
    cout<<S[i][2]<<"  "<<S[i][3]<<endl;
    cout<<endl;
    }
   
   
    return 0;
}


