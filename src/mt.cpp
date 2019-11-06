#include "../eigen-git-mirror/Eigen/Dense"
#include <iostream>
#include <list>
#include "headers.h"

void mt(const std::list<Matrix2f> sops){

    std::list<Matrix2f> mts;

    for (auto &s1: sops){
        for (auto &s2: sops){
            mts.push_back(s1*s2);
        }
    }

    int counter = 0;

    for (auto &s3: mts){
        for (auto &s4: sops){
            if (s3.isApprox(s4, 1e-4)){
                counter = counter+1;
            }
        }
    }

    if (counter==mts.size()){
        std::cout << "The group is closed" << std::endl;
        std::cout << "******************************************" << std::endl;
    }
    else std::cout << "You made a mistake !!!!" << std::endl;
}
