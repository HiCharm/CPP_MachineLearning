#ifndef DECISIONTREE
#define DECISIONTREE

#include "Mat.cpp"
#include "MLpolicy.cpp"
#include "DataLoader.cpp"
#include <memory>

template<typename T>
class Decision_tree :public ML<T> {
private:
	Mat<T> t_data;
	Mat<T> t_label;
	Mat<T> v_data;
	Mat<T> v_label;
	int n;
	int m;
	int batch;
	double end_rate;
public:
	Decision_tree() {
		n = 0;
		m = 0;
		batch = 1;
		end_rate = 0.5;
	}
	class Node {
	public:
		Mat<double> data;
		Mat<double> label;
		// 用于判断的数值大小
		double mid;
		// 用于判断的列号
		int lie;
		// 记录当前节点是0还是1
		int state = 0;
		// 记录当前是叶子
		int yezi = 0;
		shared_ptr<Node> left;
		shared_ptr<Node> right;
		Node() {
			mid = 0;
			lie = 0;
		}
		void init(Mat<double>& data_, Mat<double>& label_) {
			data = data_;
			label = label_;
			int dims = data_.get_M();
			double scores = 0;
			for (int i = 0; i < dims; i++) {
				double temp_mid = 0;
				double temp = calc_Gain(data, label, i, temp_mid);
				if (temp > scores) {
					lie = i;
					scores = temp;
					mid = temp_mid;
				}
			}
		}
		bool is_end() {
			int p = 0;
			int n = 0;
			for (int i = 0; i < label.get_N(); i++) {
				if (label.data_[i][0] == 0) {
					n++;
				}
				else {
					p++;
				}
			}
			if (n > p) state = 0;
			else state = 1;
			return (n > (n + p) * end_rate) || (p > (n + p) * end_rate);
		}
		void divide() {
			if (is_end()) {
				cout << "叶子节点结束，state位为" << state << endl;
				yezi = 1;
				return;
			}
			Mat<T> left_data;
			Mat<T> left_label;
			Mat<T> right_data;
			Mat<T> right_label;

			vector<int> p_index;
			vector<int> n_index;

			for (int i = 0; i < data.get_N();i++) {
				if (data.data_[i][lie] > mid) {
					p_index.push_back(i);
				}
				else {
					n_index.push_back(i);
				}
			}
			for (auto a : p_index) {
				right_data.add_hang(data.data_[a]);
				right_label.add_hang(label.data_[a]);
			}
			for (auto a : n_index) {
				left_data.add_hang(data.data_[a]);
				left_label.add_hang(label.data_[a]);
			}
			left = make_shared<Node>();
			right = make_shared<Node>();

			left->init(left_data, left_label);
			right->init(right_data, right_label);

			left->divide();
			right->divide();
		}
		int value(vector<double>& target) {
			if (yezi) {
				return state;
			}
			if (target[lie] > mid) {
				return right->value(target);
			}
			else {
				return left->value(target);
			}
		}


	};
	// 数据集路径，训练集比例， 是否打乱, 训练时一批次的大小, 终止比例
	void init(string csv_path, double r, bool shuffle, int batch_,double end) {
		DataLoader dl(csv_path);
		dl.set_phram(r, shuffle);
		t_data = dl.get_train_f().Z_scores();
		t_label = dl.get_train_l();
		v_data = dl.get_test_f().Z_scores();
		v_label = dl.get_test_l();
		n = t_data.get_N();
		m = t_data.get_M();
		batch = batch_;
		end_rate = end;
	}
	Node head;
	void loop(int epoch = 0) override {
		head.init(t_data, t_label);
		head.divide();
	}

	void value() override {
		double tp=0, tn=0, fp=0, fn=0;
		for (int i = 0; i < v_data.get_N(); i++) {
			int state = head.value(v_data.data_[i]);
			if (state == 1) {
				if (v_label.data_[i][0] == 1) {
					tp++;
				}
				else {
					fp++;
				}
			}
			else {
				if (v_label.data_[i][0] == 1) {
					fn++;
				}
				else {
					tn++;
				}
			}
		}
				double P = tp / (0.0+tp + fp);
		double R = tp / (0.0+tp + fn);
		double F1 = 2 * P * R / (P + R);

		cout << "P:" << P << endl;
		cout << "R:" << R << endl;
		cout << "F1:" << F1 << endl;
	}

	double calc_Gain(Mat<T>& data,Mat<T>& label,int lie, double& mid) {
		double	
			all = n,
			less_p = 0,
			less_n = 0,
			more_p = 0,
			more_n = 0,
			min_ = 9999,
			max_ = 0;
		data.get_min_and_max(lie, min_, max_);
		mid = (min_ + max_) / 2;
		for (int i = 0; i < n; i++) {
			if (data.data_[i][lie] < mid) {
				if (label.data_[i][0] == 0) {
					less_n++;
				}
				else {
					less_p++;
				}
			}
			else {
				if (label.data_[i][0] == 0) {
					more_n++;
				}
				else {
					more_p++;
				}
			}
		}
		double ans = -1 * ((less_p / all) * log(less_p / (less_p + less_n)) + (more_p / all) * log(more_p / (more_p + more_n))) - ((less_n / all) * log(less_n / (less_p + less_n)) + (more_n / all) * log(more_n / (more_p + more_n)));
		return ans;
	}
};


#endif
