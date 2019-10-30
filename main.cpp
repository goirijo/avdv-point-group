#include "Eigen/Dense"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// This will be the tolerance for floating point comparison
#define EPS 0.00001
#define M_PI 3.14159265358979323846

namespace io
{
std::vector<std::string> _parse_lattice_file(const std::string& file_path)
{
    std::ifstream file_stream;
    file_stream.open(file_path.c_str());

    std::vector<std::string> lat_values;
    std::string value;
    while (file_stream >> value)
    {
        lat_values.push_back(value);
    }

    return lat_values;
}

Eigen::Matrix2d read_lattice_from_file(const std::string& file_path)
{
    auto lat_values = _parse_lattice_file(file_path);
    Eigen::Matrix2d lattice;
    lattice(0, 0) = std::stod(lat_values[0]);
    lattice(0, 1) = std::stod(lat_values[1]);
    lattice(1, 0) = std::stod(lat_values[2]);
    lattice(1, 1) = std::stod(lat_values[3]);

    return lattice;
}
} // namespace io

namespace xtal
{

typedef Eigen::Vector2d LatticePoint;
typedef Eigen::Vector2d LatticeVector;

class Lattice
{
public:
    Lattice(const Eigen::Matrix2d& init_vectors_as_columns_matrix)
        : vectors_as_columns_matrix(init_vectors_as_columns_matrix)
    {
    }

    Lattice(const LatticeVector& init_a, const LatticeVector& init_b)
        : vectors_as_columns_matrix(Lattice::vertical_stack(init_a, init_b))
    {
    }

    LatticeVector a() const { return this->vectors_as_columns_matrix.col(0); }

    LatticeVector b() const { return this->vectors_as_columns_matrix.col(1); }

    const Eigen::Matrix2d& vectors_as_columns() const { return this->vectors_as_columns_matrix; }

    static Eigen::Matrix2d vertical_stack(const LatticeVector& a, const LatticeVector& b)
    {
        Eigen::Matrix2d mat;
        mat(0, 0) = a(0);
        mat(1, 0) = a(1);
        mat(0, 1) = b(0);
        mat(1, 1) = b(1);

        return mat;
    }

private:
    Eigen::Matrix2d vectors_as_columns_matrix;
};

/// Return a vector of lattice points by extending the lattice maxsearch times in all directions.
/// This is basically just a list of lattice points within a certain radius of the origin,
/// where the radius is specified by number of unit cells.
std::vector<LatticePoint> lattice_points_in_radius(const Lattice& lat, int search_radius)
{
    auto a_vector = lat.a();
    auto b_vector = lat.b();

    std::vector<LatticePoint> lattice_points;
    for (int i = -search_radius; i <= search_radius; ++i)
    {
        for (int j = -search_radius; j <= search_radius; ++j)
        {
            auto point = i * a_vector + j * b_vector;
            lattice_points.push_back(point);
        }
    }

    return lattice_points;
}
} // namespace xtal

namespace math
{
bool almost_equal(double val1, double val2) { return std::abs(val1 - val2) < EPS; }

bool almost_equal(const Eigen::MatrixXd& mat1, const Eigen::MatrixXd& mat2) { return mat1.isApprox(mat2, EPS); }

bool matrix_determinant_magnitude_is_unity(const Eigen::MatrixXd& mat)
{
    return almost_equal(std::abs(mat.determinant()), 1);
}

// A matrix is unitary if the conjugate transpose is also the inverse
bool matrix_is_unitary(const Eigen::MatrixXd& mat)
{
    auto identity = Eigen::MatrixXd::Identity(mat.rows(), mat.cols());
    return almost_equal(mat * mat.transpose(), identity);
}

int angle_between_vectors(const Eigen::Vector2d& vec1, const Eigen::Vector2d& vec2)
{
    auto radians=std::atan2(vec2(1),vec2(0))-std::atan2(vec1(1),vec1(0));
    return std::round(radians*(180.0/M_PI));
}
} // namespace math

namespace sym
{
typedef Eigen::Matrix2d Operation;

/// Returns the point group of the given Lattice, i.e. the group of symmetry operations
/// that maps the lattice onto itself.
std::vector<Operation> point_group(const xtal::Lattice& lattice)
{
    std::vector<Operation> point_group_operations;

    // This radius should be more than enough
    auto lattice_points = xtal::lattice_points_in_radius(lattice, 5);

    for (const auto& p1 : lattice_points)
    {
        for (const auto& p2 : lattice_points)
        {
            // Create a new lattice from the two points.
            // Each of the points effectively defines the a and b vectors of the
            // new lattice
            xtal::Lattice possible_transformed_lattice(p1, p2);
            // Find the transformation that takes you from the original lattice
            // to this new lattice. The result is a candidate symmetry operation
            const auto& orig_lat_mat = lattice.vectors_as_columns();
            const auto& possible_lat_mat = possible_transformed_lattice.vectors_as_columns();
            auto possible_symmetry_operation = possible_lat_mat * (orig_lat_mat.inverse());

            // The candidate operation is only valid if its determinant magnitude is unity
            // AND it is unitary
            if (math::matrix_determinant_magnitude_is_unity(possible_symmetry_operation) &&
                math::matrix_is_unitary(possible_symmetry_operation))
            {
                point_group_operations.push_back(possible_symmetry_operation);
            }
        }
    }

    return point_group_operations;
}

/// Enum class to categorize operations as identity, rotation, or reflection
enum class OPERATION_TYPE {IDENTITY, ROTATION, MIRROR, UNDEFINED};

/// Given a symmetry operation, determine what type of operation it is
/// by inspecting its determinant
OPERATION_TYPE categorize_operation(const Operation& operation)
{
    auto identity = Eigen::MatrixXd::Identity(operation.rows(), operation.cols());

    if(math::almost_equal(identity,operation))
    {
        return OPERATION_TYPE::IDENTITY;
    }

    auto determinant=operation.determinant();

    if(math::almost_equal(determinant,1))
    {
        return OPERATION_TYPE::ROTATION;
    }

    if(math::almost_equal(determinant,-1))
    {
        return OPERATION_TYPE::MIRROR;
    }

    return OPERATION_TYPE::UNDEFINED;
}

/// Given a symmetry operation matrix, determine the angle associated with it, e.g.
/// angle or rotation or mirror, rounded to int (0-360)
int angle_of_operation(const Operation& operation)
{
    Eigen::Vector2d ref_vec;
    ref_vec<<1,0;

    auto transformed_vec=operation*ref_vec;
    return math::angle_between_vectors(ref_vec,transformed_vec);
}

/// Given a symmetry operation, return a string that identifies it by type of operation,
/// and the associated angle of rotation/reflection
std::string label_operation(const Operation& operation)
{
    auto type=categorize_operation(operation);
    auto angle=angle_of_operation(operation);
    auto combine_to_string=[](const std::string& prefix, int angle){return prefix+std::to_string(angle);};

    std::string label;
    switch(type)
    {
        case OPERATION_TYPE::IDENTITY:
            label=combine_to_string("I",angle);
            break;
        case OPERATION_TYPE::ROTATION:
            label=combine_to_string("R",angle);
            break;
        case OPERATION_TYPE::MIRROR:
            label=combine_to_string("M",angle);
            break;
        default:
            std::cerr<<"Your symmetry operation coudn't be properly categorized. There's no recovering from this"<<std::endl;
            exit(1);
    }

    return label;
}

} // namespace sym

int main(int argc, char* argv[])
{
    std::string lat_file(argv[1]);
    auto lattice_mat = io::read_lattice_from_file(lat_file);

    xtal::Lattice lattice(lattice_mat);

    std::cout << "a:    " << lattice.a().transpose() << std::endl;
    std::cout << "b:    " << lattice.b().transpose() << std::endl;

    auto lattice_points = xtal::lattice_points_in_radius(lattice, 3);
    for (auto lp : lattice_points)
    {
        std::cout << lp.transpose() << std::endl;
    }

    std::cout << "--------------" << std::endl;

    xtal::Lattice regen(lattice.a(), lattice.b());
    std::cout << regen.vectors_as_columns() - lattice.vectors_as_columns() << std::endl;
    std::cout << "--------------" << std::endl;
    std::cout << regen.vectors_as_columns() << std::endl;
    std::cout << "--------------" << std::endl;
    std::cout << lattice.vectors_as_columns() << std::endl;

    std::cout << "--------------" << std::endl;
    auto point_group=sym::point_group(lattice);
    std::cout<<point_group.size()<<std::endl;

    std::cout << "--------------" << std::endl;
    
    Eigen::Vector2d x;
    Eigen::Vector2d y;

    x<<1,0;
    y<<1,0;
    std::cout<<math::angle_between_vectors(x,y)<<std::endl;

    x<<1,0;
    y<<0,1;
    std::cout<<math::angle_between_vectors(x,y)<<std::endl;

    x<<1,0;
    y<<-1,0;
    std::cout<<math::angle_between_vectors(x,y)<<std::endl;

    x<<1,1;
    y<<-1,1;
    std::cout<<math::angle_between_vectors(x,y)<<std::endl;

    std::cout<<math::angle_between_vectors(lattice.a(),lattice.b())<<std::endl;

    std::cout << "--------------" << std::endl;

    return 0;
}
