#include "io.hpp"
#include "math.hpp"
#include "symmetry.hpp"
#include "xtal.hpp"
#include <iostream>

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
    auto point_group = sym::point_group(lattice);
    std::cout << point_group.size() << std::endl;

    std::cout << "--------------" << std::endl;

    for (const auto& op : point_group)
    {
        std::cout << op.label() << std::endl;
        std::cout << op.cartesian_matrix() << std::endl << std::endl;
    }

    std::cout << "--------------" << std::endl;

    Eigen::Vector2d x;
    Eigen::Vector2d y;

    x << 1, 0;
    y << 1, 0;
    std::cout << math::angle_between_vectors(x, y) << std::endl;

    x << 1, 0;
    y << 0, 1;
    std::cout << math::angle_between_vectors(x, y) << std::endl;

    x << 1, 0;
    y << -1, 0;
    std::cout << math::angle_between_vectors(x, y) << std::endl;

    x << 1, 1;
    y << -1, 1;
    std::cout << math::angle_between_vectors(x, y) << std::endl;

    std::cout << math::angle_between_vectors(lattice.a(), lattice.b()) << std::endl;

    std::cout << "--------------" << std::endl;

    if (sym::group_is_colsed(point_group))
    {
        std::cout << "The group is closed." << std::endl;
    }

    else
    {
        std::cout << "The group is NOT closed !!!" << std::endl;
        for (const auto& op : point_group)
        {
            std::cout << op.label() << std::endl;
        }
    }

    return 0;
}
