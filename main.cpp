
#include "Logistic.cpp"
#include "Decision_tree.cpp"

int main() {
	string csv_path = "D:\\work\\2025\\CPP_ML\\开源数据集\\4_客户流失预测.csv";


	//Logistic<double> Logi;

	//Logi.init(csv_path, 0.0001, 0.9, true, 512);
	//Logi.loop(30);
	//Logi.value();

	Decision_tree<double> dctree;
	dctree.init(csv_path, 0.8, 0, 64, 0.5);
	dctree.loop();
	dctree.value();
	


	return 0;
}