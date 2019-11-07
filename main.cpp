#include "io.hpp"
#include "math.hpp"
#include "symmetry.hpp"
#include "xtal.hpp"
#include <iostream>

void big_divider()
{
    std::cout<<"-----------------------------------------------------"<<std::endl;
    return;
}

void small_divider()
{
    std::cout<<"----------------"<<std::endl;
    return;
}

int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        std::cout<<"Please provide exactly one file containing a lattice as an argument."<<std::endl;
        return 1;
    }

    std::string lat_file(argv[1]);
    auto lattice=xtal::Lattice::from_file(lat_file);

    std::cout<<"The lattice vectors are:"<<std::endl;
    std::cout << "a:    " << lattice.a().transpose() << std::endl;
    std::cout << "b:    " << lattice.b().transpose() << std::endl;
    big_divider();

    //----------------------------------------------------------------------

    auto point_group = sym::make_point_group(lattice);

    std::cout<<"The point group operations are:"<<std::endl<<std::endl;
    small_divider();
    for (const auto& op : point_group)
    {
        std::cout << op << std::endl;
        std::cout << op.cartesian_matrix() << std::endl << std::endl;
        small_divider();
    }
    big_divider();

    //----------------------------------------------------------------------

    if (sym::group_is_closed(point_group))
    {
        std::cout << "The group is closed." << std::endl;
    }

    else
    {
        std::cout << "The group is NOT closed!!!" << std::endl;
    }

    //----------------------------------------------------------------------

    std::cout<<"The multiplication table is:"<<std::endl;

    auto multiplication_table=sym::make_multiplication_table(point_group);
    io::print_formatted_table(std::cout, multiplication_table, 6);

    big_divider();

    //----------------------------------------------------------------------

    return 0;
}

int testing(int argc, char* argv[])
{
    std::string lat_file(argv[1]);
    auto lattice=xtal::Lattice::from_file(lat_file);

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
    auto point_group = sym::make_point_group(lattice);
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
    auto undefined_operation=sym::Operation::undefined();
    std::cout<<undefined_operation.label()<<std::endl;
    std::cout << "--------------" << std::endl;

    if (sym::group_is_closed(point_group))
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

    auto multiplication_table=sym::make_multiplication_table(point_group);
    io::print_formatted_table(std::cout, multiplication_table, 6);

    return 0;
}
