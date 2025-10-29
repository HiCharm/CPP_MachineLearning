#ifndef MLPOLICY
#define MLPOLICY

template<typename T>
class ML {
public:
	// ÆÀ¹À
	virtual void value() = 0;
	// Ñ§Ï°Ñ­»·
	virtual void loop(int epoch) = 0;
};

#endif