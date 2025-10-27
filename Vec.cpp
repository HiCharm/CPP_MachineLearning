#ifndef VECCPP
#define VECCPP
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>

using namespace std;

template<typename T>
class Vec;

template<typename T>
Vec<T> operator*(const T& BL, const Vec<T>& XL);
template<typename T>
Vec<T> operator*(const Vec<T>& XL, const T& BL);
template<typename T>
T operator*(const Vec<T>& XL1, const Vec<T>& XL2);

template<typename T>
class Vec {
private:
	// 维度
	int N_;

	// 数据结构
	vector<T> data_;
	
	// 简写
	using vecT = Vec<T>;

public:
	// 默认构造函数
	Vec(int N = 0) {
		N_ = N;
		data_.resize(N);
	}
	// 拷贝构造函数
	Vec(const Vec<T>& b) {
		N_ = b.N_;
		data_ = b.data_;
	}
	// 移动构造函数
	Vec(Vec<T>&& b) {
		N_ = b.N_;
		data_ = move(b.data_);
	}
	// 析构
	~Vec() {
		data_.clear();
	}

	// set函数部分
	//	注意：只能传递右值
	void set(int index, T &&val) { data_[index] = val; };

	// get函数部分
	int get_N() { return N_; };
	T get_V(int index) { return data_[index]; };
	vector<T> get_D() { return data_; };

	// 模的计算
	double calc_mo() {
		double res = 0;
		for (int i = 0; i < N_; i++) {
			res += data_[i] * data_[i];
		}
		return sqrt(res);
	}

	// 重载运算符部分
	
	// 加法重载
	vecT operator+(const vecT& b) {
		vecT ans(b.N_);
		for (int i = 0; i < b.N_; i++) {
			ans.set(i, data_[i] + b.data_[i]);
		}
		return ans;
	}

	// 减法重载
	vecT operator-(const vecT& b) {
		vecT ans(b.N_);
		for (int i = 0; i < b.N_; i++) {
			ans.set(i, data_[i] - b.data_[i]);
		}
		return ans;
	}

	// 等号重载
	vecT& operator=(const vecT& b) {
		if (&b != this) {
			N_ = b.N_;
			data_ = b.data_;
		}
		return *this;
	}

	friend Vec<T> operator*<>(const T& BL, const Vec<T>& XL);
	friend Vec<T> operator*<>(const Vec<T>& XL, const T& BL);
	friend T operator*<>(const Vec<T>& XL1, const Vec<T>& XL2);

	// 向量夹角的计算
	double calc_angle_cos(const vecT& XL) {
		return (XL * (*this)) / (calc_mo() * XL.calc_mo());
	}
};

// 数乘运算
template<typename T>
Vec<T> operator*(const T& BL, const Vec<T>& XL) {
	Vec<T> res(XL.N_);
	for (int i = 0; i < XL.N_; i++) {
		res.set(i, BL * XL.data_[i]);
	}
	return res;
}

template<typename T>
Vec<T> operator*(const Vec<T>& XL, const T& BL) {
	return BL * XL;
}

// 点积运算
template<typename T>
T operator*(const Vec<T>& XL1, const Vec<T>& XL2) {
	T res = 0;
	for (int i = 0; i < XL1.N_; i++) {
		res += XL1.data_[i] * XL2.data_[i];
	}
	return res;
}

/*
int main() {
	Vec<double> v1(10);
	Vec<double> v2(10);
	for (int i = 0; i < 10; i++) {
		v1.set(i, i + 1);
		v2.set(i, 2 * i);
	}
	Vec<double> v3;

	v3 = 3.0 * v2;

	for (auto a : v3.get_D()) {
		cout << a << " ";
	}
	cout << endl;

	cout << v1 * v2 << endl;

	return 0;
}
*/

#endif