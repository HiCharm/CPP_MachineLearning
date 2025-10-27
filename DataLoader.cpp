#ifndef DATALOADER
#define DATALOADER

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <algorithm>
#include "Mat.cpp"

using namespace std;

class DataLoader {
private:
	vector<vector<double>> data;
	vector<string> title;
	int N_;
	int M_;
	// 分割比例
	double ratio=0;
	// 索引
	vector<int> index;
public:
	DataLoader(string &csv_path) {
		ifstream csv_data(csv_path, ios::in);

		istringstream sin;
		string line;
		string word;

		getline(csv_data, line);

		// 读取标题行
		sin.str(line);
		while (getline(sin, word, ',')) {
			title.push_back(word);
		}
		//

		int i = 0;
		int j = 0;
		while (getline(csv_data, line)) {
			word.clear();
			sin.clear();

			sin.str(line);
			j = 0;
			data.push_back(vector<double>(0));
			while (getline(sin, word, ',')) {

				data[i].push_back(stod(word));
				j++;
			}
			i++;
		}

		N_ = i;
		M_ = j;

		csv_data.close();
	}

	// r:分割比例，shuffle:是否打乱
	void set_phram(double r, bool shuffle_) {
		ratio = r;
		index.resize(N_);
		for (int i = 0; i < N_;i++) {
			index[i] = i;
		}
		if (shuffle_ == true) {
			random_device rd;
			mt19937 g(rd());
			shuffle(index.begin(), index.end(), g);
		}
	}
	Mat<double> get_train_f() {
		int div = N_ * ratio;
		Mat<double> ans(div, M_ - 1);
		for (int i = 0; i < div; i++) {
			for (int j = 0; j < (M_ - 1); j++) {
				ans.set(i, j, data[index[i]][j]);
			}
		}
		return ans;
	}
	Mat<double> get_train_l() {
		int div = N_ * ratio;
		Mat<double> ans(div, 1);
		for (int i = 0; i < div; i++) {
			for (int j = M_-1; j < M_; j++) {
				ans.set(i, 0, data[index[i]][j]);
			}
		}
		return ans;
	}
	Mat<double> get_test_f() {
		int div = N_ * ratio;
		Mat<double> ans(N_ - div, M_ - 1);
		for (int i = div; i < N_; i++) {
			for (int j = 0; j < (M_ - 1); j++) {
				ans.set(i-div, j, data[index[i]][j]);
			}
		}
		return ans;
	}
	Mat<double> get_test_l() {
		int div = N_ * ratio;
		Mat<double> ans(N_ - div, 1);
		for (int i = div; i < N_; i++) {
			for (int j = M_ - 1; j < M_; j++) {
				ans.set(i - div, 0, data[index[i]][j]);
			}
		}
		return ans;
	}
};
#endif

//int main() {
//	string csv_path = R"(D:\work\2025\CPP_ML\开源数据集\4_客户流失预测.csv)";
//	DataLoader dl(csv_path);
//
//	dl.set_phram(0.5, 1);
//
//	Mat<double> train_f, train_l, test_f, test_l;
//	train_f = dl.get_train_f();
//	train_l = dl.get_train_l();
//	test_f = dl.get_test_f();
//	test_l = dl.get_test_l();
//
//	print(train_f);
//	print(train_l);
//	print(test_f);
//	print(test_l);
//
//	return 0;
//}