
#include "Logistic.cpp"
#include "Decision_tree.cpp"
#include "SVM.cpp"

int main() {
	string csv_path = "D:\\work\\2025\\CPP_ML\\开源数据集\\4_客户流失预测.csv";


	//Logistic<double> Logi;

	//Logi.init(csv_path, 0.0001, 0.9, true, 512);
	//Logi.loop(30);
	//Logi.value();

	//Decision_tree<double> dctree;
	//dctree.init(csv_path, 0.8, 0, 64, 0.5);
	//dctree.loop();
	//dctree.value();
	
	SVM<double> mySVM;

	mySVM.init(csv_path, 0.0001, 0.8, false, 1e-5, 100);
	mySVM.loop(1000);
	mySVM.value();

	return 0;
}