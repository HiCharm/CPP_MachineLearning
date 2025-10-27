#ifndef MATCPP
#define MATCPP

#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class Mat;

template<typename T>
Mat<T> operator*(const T& BL, const Mat<T>& JZ);

template<typename T>
class Mat {
private:
	int N_;// ��
	int M_;// ��
	using matT = vector<vector<T>>;
	using vecT = vector<T>;
	matT data_;
public:
	// ���캯��
	Mat(int N = 0, int M = 0) {
		data_.resize(N, vecT(M));
		N_ = N;
		M_ = M;
	}
	// �������캯��
	Mat(const Mat& other) {
		data_.clear();
		data_ = other.data_;
		N_ = other.N_;
		M_ = other.M_;
	}
	// �ƶ����캯��
	Mat(const Mat&& other) {
		data_ = move(other.data_);
		N_ = other.N_;
		M_ = other.M_;
	}
	~Mat() {
		data_.clear();
	}

	Mat<T>& operator=(const Mat<T>& b) {
		if (&b != this) {
			N_ = b.N_;
			M_ = b.M_;
			data_ = b.data_;
		}
		return *this;
	}

	// get��������
	int get_N() { return N_; }
	int get_M() { return M_; }
	matT get_D() { return data_; }
	T get_T(int hang, int lie) { return data_[hang][lie]; }

	// set��������
	void set(int hang, int lie, T val) {
		data_[hang][lie] = val;
	}

	// ����ӷ�
	Mat<T> operator+(Mat<T>& b) {
		Mat<T> ans(b.N_,b.M_);
		for (int i = 0; i < b.N_; i++) {
			for (int j = 0; j < b.M_; j++) {
				ans[i][j] = data_[i][j] + b.data_[i][j];
			}
		}
		return ans;
	}

	// �������
	Mat<T> operator-(Mat<T>& b) {
		Mat<T> ans(b.N_, b.M_);
		for (int i = 0; i < b.N_; i++) {
			for (int j = 0; j < b.M_; j++) {
				ans[i][j] = data_[i][j] - b.data_[i][j];
			}
		}
		return ans;
	}

	// ��������
	friend Mat<T> operator*<>(const T& BL, const Mat<T>& JZ);

	// ����˷�
	Mat<T> operator*(const Mat<T>& b) {
		if (M_ != b.N_) {
			throw "����˷����Ϸ�";
		}
		Mat<T> ans(N_, b.M_);
		for (int i = 0; i < N_; i++) {
			for (int j = 0; j < b.M_; j++) {
				double sum = 0;
				for (int k = 0; k < M_; k++) {
					sum += data_[i][k] * b.data_[k][j];
				}
				ans.set(i, j, sum);
			}
		}
		return ans;
	}
};

// ��������
template<typename T>
Mat<T> operator*(const T& BL, const Mat<T>& JZ) {
	Mat<T> ans(JZ);
	for (int i = 0; i < JZ.N_; i++) {
		for (int j = 0; j < JZ.M_; j++) {
			ans.set(i, j, ans.get_T(i, j) * BL);
		}
	}
	return ans;
}

// �����ӡ
template<typename T>
void print(Mat<T>& a) {
	cout << "Mat" << endl;
	for (int i = 0; i < a.get_N(); i++) {
		for (int j = 0; j < a.get_M(); j++) {
			cout << a.get_T(i, j) << " ";
		}
		cout << endl;
	}
}

//int main() {
//	// ���Ծ���˷�
//
//	// ����һ��ȫ1����
//	Mat<int> m1(3, 3);
//	for (int i = 0;i < 3; i++) {
//		for (int j = 0; j < 3; j++) {
//			m1.set(i, j, 1);
//		}
//	}
//
//	Mat<int> m2(3, 4);
//	for (int i = 0;i < 3; i++) {
//		for (int j = 0; j < 4; j++) {
//			m2.set(i, j, 2);
//		}
//	}
//
//	print(m1);
//
//	m1 = 3 * m1;
//
//	print(m1);
//	print(m2);
//
//	m2 = m1 * m2;
//
//	print(m2);
//
//	return 0;
//}

#endif