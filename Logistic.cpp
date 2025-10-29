#ifndef LOGISTIC
#define LOGISTIC

#include "DataLoader.cpp"
#include "Mat.cpp"
#include "MLpolicy.cpp"
#include "Sigmod.cpp"


template<typename T>
class Logistic:public ML<T> {
private:
	Mat<T> t_data;
	Mat<T> t_label;
	Mat<T> v_data;
	Mat<T> v_label;
	int n;
	int m;
	Mat<T> w;
	double lr = 0;;
	double judge_line = 0.5;

public:
	Logistic() {
		n = 0;
		m = 0;
	}
	// 数据集路径， 训练集比例， 是否打乱
	void init(string csv_path,double lr_, double r, bool shuffle){
		DataLoader dl(csv_path);
		dl.set_phram(r, shuffle);
		t_data = dl.get_train_f().Z_scores();
		t_label = dl.get_train_l();
		v_data = dl.get_test_f().Z_scores();
		v_label = dl.get_test_l();
		n = t_data.get_N();
		m = t_data.get_M();
		lr = lr_;
	}

	void loop(int epoch) override {
		t_data.add_lie(1.0);
		w = Mat<double>(t_data.get_M(), 1,1.0);
		Mat<T> err;
		for (int i = 0; i < epoch; i++) {
			Mat<T> h = t_data * w;
			h = sigmod_mat(h);
			h = h.judge(judge_line);
			err = t_label - h;
			cout << "epoch: " << i << endl;
			loss(err);
			w = w + lr  * t_data.T_() * err;
		}
	}

	void loss(Mat<T>& err) {
		double sum = 0;
		for (int i = 0; i < err.get_N(); i++) {
			sum += abs(err.get_T(i, 0));
		}
		cout << "loss: " << sum << endl;
	}

	void value()override {
		
		v_data.add_lie(1.0);

		Mat<T> h = v_data * w;
		h = sigmod_mat(h);

		int tp=0, fp=0, tn=0, fn=0;

		double h_, lb;
		for (int i = 0;i < h.get_N(); i++) {
			h_ = h.get_T(i, 0);
			if (h_ > judge_line) {
				h_ = 1;
			}
			else {
				h_ = 0;
			}
			lb = v_label.get_T(i, 0);

			if (h_ == 1 && lb == 1) {
				tp++;
			}
			else if (h_ == 1 && lb == 0) {
				fp++;
			}
			else if (h_ == 0 && lb == 0) {
				tn++;
			}
			else {
				fn++;
			}
		}
		double P = tp / (0.0+tp + fp);
		double R = tp / (0.0+tp + fn);
		double F1 = 2 * P * R / (P + R);

		cout << "P:" << P << endl;
		cout << "R:" << R << endl;
		cout << "F1:" << F1 << endl;
	}
};

#endif