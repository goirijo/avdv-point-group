#include "Eigen/Core"
#include "Eigen/src/Core/Matrix.h"

class Symmetry_Operation
{

public:
    Symmetry_Operation(Eigen::Matrix3d cart_matrix,  Eigen::Vector3d translation);
    Eigen::Matrix3d cart_matrix;
    Eigen::Vector3d translation;

};
