#ifndef SIGMOD
#define SIGMOD

#include "Mat.cpp"
#include <cmath>

//double sigmod(double x) {
//	return 1 / (1 + exp(-1 * x));
//}

template<typename T>
Mat<T> sigmod_mat(Mat<T>& a) {
	Mat<T> ans(a.get_N(), a.get_M());
	for (int i = 0; i < ans.get_N(); i++) {
		for (int j = 0; j < ans.get_M(); j++) {
			ans.set(i, j, 1.0/(1+exp(-1*a.get_T(i, j))));
		}
	}
	return ans;
}

#endif