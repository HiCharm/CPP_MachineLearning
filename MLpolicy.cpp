#ifndef MLPOLICY
#define MLPOLICY

template<typename T>
class ML {
public:
	// ����
	virtual void value() = 0;
	// ѧϰѭ��
	virtual void loop(int epoch) = 0;
};

#endif