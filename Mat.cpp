#ifndef MATCPP
#define MATCPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

template<typename T>
class Mat;

template<typename T>
Mat<T> operator*(const T& BL, const Mat<T>& JZ);

template<typename T>
class Mat {
private:
	int N_;// 行
	int M_;// 列
	using matT = vector<vector<T>>;
	using vecT = vector<T>;
	matT data_;
public:
	// 构造函数
	Mat() {
		N_ = 0;
		M_ = 0;
	}
	Mat(int N, int M) {
		data_.resize(N, vecT(M));
		N_ = N;
		M_ = M;
	}
	Mat(int N, int M, T val) {
		data_.resize(N, vecT(M, val));
		N_ = N;
		M_ = M;
	}
	// 拷贝构造函数
	Mat(const Mat& other) {
		data_.clear();
		data_ = other.data_;
		N_ = other.N_;
		M_ = other.M_;
	}
	// 移动构造函数
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

	// get函数部分
	int get_N() { return N_; }
	int get_M() { return M_; }
	matT get_D() { return data_; }
	T get_T(int hang, int lie) { return data_[hang][lie]; }

	// set函数部分
	void set(int hang, int lie, T val) {
		data_[hang][lie] = val;
	}

	// 矩阵加法
	Mat<T> operator+(Mat<T>& b) {
		Mat<T> ans(b.N_,b.M_);
		for (int i = 0; i < b.N_; i++) {
			for (int j = 0; j < b.M_; j++) {
				ans.data_[i][j] = data_[i][j] + b.data_[i][j];
			}
		}
		return ans;
	}Mat<T> operator+(Mat<T>&& b) {
		Mat<T> ans(b.N_, b.M_);
		for (int i = 0; i < b.N_; i++) {
			for (int j = 0; j < b.M_; j++) {
				ans.data_[i][j] = data_[i][j] + b.data_[i][j];
			}
		}
		return ans;
	}

	// 矩阵减法
	Mat<T> operator-(Mat<T>& b) {
		Mat<T> ans(b.N_, b.M_);
		for (int i = 0; i < b.N_; i++) {
			for (int j = 0; j < b.M_; j++) {
				ans.data_[i][j] = data_[i][j] - b.data_[i][j];
			}
		}
		return ans;
	}
	Mat<T> operator-(Mat<T>&& b) {
		Mat<T> ans(b.N_, b.M_);
		for (int i = 0; i < b.N_; i++) {
			for (int j = 0; j < b.M_; j++) {
				ans.data_[i][j] = data_[i][j] - b.data_[i][j];
			}
		}
		return ans;
	}
	// 矩阵数乘
	friend Mat<T> operator*<>(const T& BL, const Mat<T>& JZ);

	// 矩阵乘法
	Mat<T> operator*(const Mat<T>& b) {
		if (M_ != b.N_) {
			throw "矩阵乘法不合法";
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

	// 矩阵转置
	Mat<T> T_() {
		Mat<T> ans(M_,N_);
		for (int i = 0; i < N_; i++) {
			for (int j = 0; j < M_; j++) {
				ans.data_[j][i] = data_[i][j];
			}
		}
		return ans;
	}

	// 矩阵求行列式
	T det() {
		if (N_ != M_) {
			throw "非方阵不能算行列式";
		}
		if (N_ == 2) {
			return data_[0][0] * data_[1][1] - data_[1][0] * data_[0][1];
		}
		T ans = 0;
		// 默认按第一行展开
		for (int i = 0; i < M_; i++) {
			ans += data_[0][i]* Cij(0, i);
		}
		return ans;
	}

	// 矩阵求代数余子式
	T Cij(int hang, int lie) {
		Mat<T> ans(N_ - 1, M_ - 1);
		int k = 0;
		for (int i = 0; i < N_; i++) {
			for (int j = 0; j < M_; j++) {
				if (i == hang || j == lie) {
					continue;
				}
				ans.data_[k / (M_ - 1)][k % (M_ - 1)] = data_[i][j];
				k++;
			}
		}
		return ((hang + lie) % 2 == 0 ? 1 : -1) * ans.det();
	}

	// 矩阵求逆
	Mat<T> ni() {
		Mat<T> ans(N_, M_);
		T det_a = this->det();
		if (det_a == 0) {
			throw "行列式为0，不能求逆";
		}
		for (int i = 0; i < N_; i++) {
			for (int j = 0; j < M_; j++) {
				ans.data_[i][j] = Cij(i, j) / det_a;
			}
		}
		return ans;
	}

	// 矩阵增加一列
	void add_lie(T val) {
		M_++;
		for (auto& a : data_) {
			a.push_back(val);
		}
	}

	// 矩阵求均值
	Mat<T> calc_means() {
		Mat<T> ans(1, M_);
		if (N_ == 0 || M_ == 0) {
			return ans;
		}
		for (int i = 0; i < M_; i++) {
			T sum = 0;
			for (int j = 0; j < N_; j++) {
				sum += data_[j][i];
			}
			ans.data_[0][i] = sum / N_;
		}
		return ans;
	}

	// 矩阵求标准差
	Mat<T> calc_stds() {
		Mat<T> ans(1, M_);
		if (N_ == 0 || M_ == 0) {
			return ans;
		}
		Mat<T> means = calc_means();
		for (int i = 0; i < M_; i++) {
			T sum_sq = 0;
			for (int j = 0; j < N_; j++) {
				T diff = data_[j][i] - means.data_[0][i];
				sum_sq += diff * diff;
			}
			ans.data_[0][i] = sqrt(sum_sq / N_);
		}
		return ans;
	}

	Mat<T> Z_scores() {
		Mat<T> ans(N_, M_);
		if (N_ == 0 || M_ == 0) {
			throw "矩阵是空的";
		}
		Mat<T> means = calc_means();
		Mat<T> stds = calc_stds();

		for (int j = 0; j < M_; j++) {
			T std = stds.data_[0][j];
			for (int i = 0; i < N_; i++) {
				if (std == 0) {
					ans.data_[i][j] = 0;
				}
				else {
					ans.data_[i][j] = (data_[i][j] - means.data_[0][j]) / std;
				}
			}
		}
		return ans;
	}

	Mat<T> judge(double a) {
		Mat<T> ans(N_, M_);
		for (int i = 0; i < N_; i++) {
			for (int j = 0; j < M_; j++) {
				if (data_[i][j] > a) {
					ans.data_[i][j] = 1;
				}
				else {
					ans.data_[i][j] = 0;
				}
			}
		}
		return ans;
	}
};

// 矩阵数乘
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

// 矩阵打印
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
//	// 测试矩阵乘法
//
//	// 创建一个矩阵
//	Mat<double> m1(3, 3);
//	for (int i = 0;i < 3; i++) {
//		for (int j = 0; j < 3; j++) {
//			m1.set(i, j, (i==j?1:0));
//		}
//	}
//
//	Mat<double> m2(3, 4);
//	for (int i = 0;i < 3; i++) {
//		for (int j = 0; j < 4; j++) {
//			m2.set(i, j, 2);
//		}
//	}
//
//	print(m1);
//
//	m1 = 3.0 * m1;
//
//	print(m1);
//
//	cout << "m1的行列式 " << m1.det() << endl;
//	print(m1);
//	Mat<double> m3 = m1.ni();
//	print(m3);
//
//	print(m2);
//
//	m2 = m1 * m2;
//
//	print(m2);
//
//	m2 = m2.T_();
//
//	print(m2);
//
//
//
//	return 0;
//}

#endif