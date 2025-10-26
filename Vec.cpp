#include <iostream>
#include <vector>
#include <memory>

using namespace std;

// ����һ�������Ķ�����
/*
��Ҫ��
	1���Զ���ά��
	2�����
	3�����
	4��
*/

//using Mat = shared_ptr<vector<vector<double>>>;
//using vec = shared_ptr<vector<double>>;
using Mat = vector<vector<double>>;
using vec = vector<double>;

class Vec {
public:
	int N;
	vec vec_ = vec(N);
	Vec(int N_) {
		N = N_;
	}
	//~Vec();

	//double L2(vec& a);
	void set(int index, double val) {
		vec_[index] = val;
	}
	double dot(vec a, vec b) {
		double ans = 0;
		for (int i = 0; i < a.size(); i++) {
			ans += a[i] * b[i];
		}
		return ans;
	}
	//vec chacheng(vec& a, vec& b);

};

// ����̳�����
class Matrix : public Vec{
public:
	int N;// hang
	int M;
	Mat mat_ = Mat(N, vec(M));
	Matrix(int N, int M);
	~Matrix();

	//double hanglieshi();
	void set(int hang, int lie, double val) {
		mat_[hang][lie] = val;
	}
	Matrix mutlipy(Matrix& a, Matrix& b);
	Matrix T();
	vec getvec(int i) {
		return mat_[i];
	}
	//Mat qiuni(Mat& a);
	//Mat qiuzhi(Mat& a);

};

Matrix Matrix::mutlipy(Matrix& a, Matrix& b) {
	if (a.M != b.N) {
		throw "����˷����Ϸ�";
	}
	b = b.T();
	Matrix ans(a.N, b.M);

	for (int i = 0; i < a.N; i++) {
		for (int j = 0; j < b.N; j++) {
			ans.set(i, j, dot(a.getvec(i),b.getvec(j)));
		}
	}
	
	return ans;

}

