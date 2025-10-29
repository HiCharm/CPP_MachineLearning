
#include "Logistic.cpp"



int main() {
	string csv_path = "D:\\work\\2025\\CPP_ML\\开源数据集\\4_客户流失预测.csv";
	Logistic<double> Logi;
	Logi.init(csv_path, 0.00001 ,0.8, false);
	Logi.loop(1000);
	Logi.value();
	return 0;
}