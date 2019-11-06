
#include <iostream>
#include <cmath>
#include <vector>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;


// Checks the validity of a symmetry operation matrix.
bool is_sym_op_valid(std::vector<double> S) {
	if (S.determinant() != 1 || S.determinant() != -1) {
		return false;
	}
	if (S.transpose()*S != 1) {
		return false;
	}
	return true; 
}

//Creates grid points from lattice vectors a, b and a radius.
std::vector<double> create_grid_points(std::vector<double> a, std::vector<double> b, int radius) {
	std::vector<int> list_gp = {};
	for (int n = -radius; n = radius; n++) {
		for (int m = -radius; m = radius; m++) {
			std::vector<double> p = n * a + m * b;
			list_gp.push_back(p);
		}
	}
	return list_gp;
}

//Calculates a list of L primes based on a lattice grid. 
std::vector<double> calc_L_primes(vector<double> a, vector<double> b) {
	std::vector<double> list_Lps = {};
	std::vector<double> ps = create_grid_points(vector<double> a, vector<double> b, int radius);
	for (int p1 = 0; p1 < ps.size(); p1++) {
		for (int p2 = 0; p2 < ps.size(); p2++) {
			std::vector<double> Lp = {p1, p2};
			list_Lps.push_back(Lp);
		}
	}
	return list_Lps;
}

//Calculates a point group (a list of symmetry operations) from L primes.
std::vector<double> calc_point_group(vector<double> a, vector<double> b) {
	std::vector<double> list_sym_ops = {};
	std::vector<double> ps = calc_grid_points(vector<double> a, vector<double> b, int radius);
	std::vector<double> Lps = calc_L_primes(ps);
	for (int Lp = 0; Lp < Lps.size(); Lp++) {
		std::vector<double> S = Lp * Lp.inverse();
		if (is_sym_op_valid(S) == true) {
			list_sym_ops.push_back(S);
		}
	}
	return list_sym_ops;
}

//Checks the validity of the list of symmetry operations as a group by checking for closure. 
bool group_is_closed(std::vector<double> list_sym_ops) {
	for (int s1 = 0; s1 < list_sym_ops.size(); s1++) {
		for (int s2 = 0; s2 < list_sym_ops.size(); s2++) {
			std::vector<double> Sp = s1 * s2;
			if (Sp != list_sym_ops) {
				return false;
			}
		}
	}
	return true;
}
