#ifndef SVMML
#define SVMML

#include "DataLoader.cpp"
#include "Mat.cpp"
#include "MLpolicy.cpp"

template<typename T>
class SVM : public ML<T> {
private:
	Mat<T> t_data;
	Mat<T> t_label;
	Mat<T> v_data;
	Mat<T> v_label;
	int n;
	int m;
	Mat<T> w;
	double b;
	double lr = 0;
	double end_tol = 1e-6;
	int max_iter = 100000;

public:
	SVM() {
		n = 0;
		m = 0;
		b = 0;
	}
	// 数据集路径， 学习率，训练集比例， 是否打乱, 临界终止条件，最大迭代次数
	void init(string csv_path, double lr_, double r, bool shuffle, double end, int maxiter = 100000) {
		DataLoader dl(csv_path);
		dl.set_phram(r, shuffle);
		t_data = dl.get_train_f().Z_scores();
		t_label = dl.get_train_l();
		v_data = dl.get_test_f().Z_scores();
		v_label = dl.get_test_l();
		n = t_data.get_N();
		m = t_data.get_M();
		lr = lr_;
		end_tol = end;
		max_iter = maxiter;

		t_label.judge(1, -1, 1, false);
		v_label.judge(1, -1, 1, false);

	}
	void loop(int epoch = max_iter) override {
		Mat<T> Q(n, n);
		Mat<T> C(n, 1, -1.0);
		Mat<T> G;
		Mat<T> lambda(n, 1, 0.0);
		Mat<T> n_lambda;
		double phi = 0;
		Mat<T> delta;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				Q.data_[i][j] = (t_label.data_[i][0] * t_label.data_[j][0]) * (t_data.dot(t_data, i, j));
			}
		}

		int times = 0;
		while (times < max_iter) {
			G = Q * lambda + C;
			n_lambda = lambda - lr * G;
			n_lambda.judge(0, 0, 1, false);
			/*phi = n_lambda * t_label;*/
			//phi = 0;
			//for (int i = 0; i < n; i++) {
			//	phi += n_lambda.data_[i][0] * t_label.data_[i][0];
			//}
			//if (phi > 1e-5) {
			//	n_lambda = lambda - (phi / n) * t_label;
			//}
			delta = n_lambda - lambda;
			if (delta.L2(0) < end_tol) {
				break;
			}
			lambda = n_lambda;
			//cout << "循环迭代 " << times << " 代, 偏差为:" << phi << endl;
			cout << "循环迭代 " << times << endl;
			times++;
		}

		Mat<T> SY(1, n);
		for (int i = 0; i < n; i++) {
			SY.data_[0][i] = lambda.data_[i][0] * t_label.data_[i][0];
		}

		w = SY * t_data;

		for (int i = 0; i < n; i++) {
			if (lambda.data_[i][0] > 0) {
				b = t_label.data_[i][0] - w.dot(t_data, 0, i);
			}
		}
	}

	double sign(double x) {
		return x >= 1 ? 1 : -1;
	}

	void value()override {

		int vn = v_data.get_N();

		Mat<T> h(vn, 1);

		for (int i = 0; i < vn; i++) {
			h.data_[i][0] = sign(w.dot(v_data, 0, i) + b);
		}

		int tp = 0, fp = 0, tn = 0, fn = 0;
		cout << "正在评估模型指标" << endl;
		double h_, lb;
		for (int i = 0;i < h.get_N(); i++) {
			h_ = h.data_[i][0];
			lb = v_label.data_[i][0];

			if (h_ == 1 && lb == 1) {
				tp++;
			}
			else if (h_ == 1 && lb == -1) {
				fp++;
			}
			else if (h_ == -1 && lb == -1) {
				tn++;
			}
			else {
				fn++;
			}
		}
		double P = tp / (0.0 + tp + fp);
		double R = tp / (0.0 + tp + fn);
		double F1 = 2 * P * R / (P + R);

		cout << "P:" << P << endl;
		cout << "R:" << R << endl;
		cout << "F1:" << F1 << endl;
	}
};


#endif