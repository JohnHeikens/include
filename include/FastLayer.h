#pragma once
#include "mathfunctions.h"

//https://stackoverflow.com/questions/10732027/fast-sigmoid-algorithm
	//https://rechneronline.de/function-graphs/
	//range[0,1]
inline fp sigmoid01(const fp& x)
{
	return 0.5 + //bring it from range [-1, 1] to [0, 1]
		x / (1 + abs(x))
		* 0.5;
}
//https://www.derivative-calculator.net/
//derivative of sigmoid01
inline fp sigmoid01_d(const fp& x)
{
	return 1 / (2 * math::squared(abs(x) + 1));
}
inline fp reLu(const fp& x)
{
	return x > 0 ? x : 0;
}
inline fp reLu_d(const fp& x)
{
	return x > 0 ? 1 : 0;
}
cfp leakyRelumult = 1.0 / 5.5;//0.01;
inline fp leakyReLu(const fp& x)
{
	return x > 0 ? x : x * leakyRelumult;
}
inline fp leakyReLu_d(const fp& x)
{
	return x > 0 ? 1 : leakyRelumult;
}
cfp treshold = 6;
inline fp leakyReLu6(const fp& x)
{
	return x > 0 ? x < treshold ? x : treshold + x * leakyRelumult : x * leakyRelumult;
}
inline fp leakyReLu6_d(const fp& x)
{
	return x > 0 ? x < treshold ? 1 : leakyRelumult : leakyRelumult;
}
inline fp checkClamp(cfp& newValue) 
{
	return clamp(newValue, (fp)-1.0, (fp)1.0);
}
inline fp linear(cfp& newValue) 
{
	return newValue;
}

extern fp(*activationFunction)(cfp& x);
extern fp(*activationFunctionDer)(cfp& x);

extern fp(*activationFunctionEnd)(cfp& x);
extern fp(*activationFunctionDerEnd)(cfp& x);

extern fp(*checkWeights)(cfp& x);

struct FastLayer
{
	int lastLayerCount;//the amount of neurons that 'stop' from now
	int neuronCount, UpperLayerNeuronCountBias;
	FastLayer* const upperLayer;
	//weight matrix layout:
	//y: bias + upper layer neurons
	//x: current layer neurons
	fp* weightmatrix = nullptr;
	fp* values = nullptr;
	fp* totalSums = nullptr;

	//CAUTION! THE WEIGHTS LIST IS NOT INITIALIZED!
	//BOTH CONSTRUCTORS DONT INITIALIZE TOTALSUMS EITHER
	FastLayer(FastLayer* const upperLayer, cint& neuronCount, FastLayer* const reproduceFrom, const fp& alpha);
	FastLayer(FastLayer* const upperLayer, cint& neuronCount, cint& lastLayerCount);
	static FastLayer* randomLayer(FastLayer* const upperLayer, cint& neuronCount, cint& lastLayerCount);
	FastLayer* clone(FastLayer* const upperLayer);
	void Evaluate();

	void calculateImpact(fp* curValuesImpact, cbool& end, fp*& weightsImpact, fp*& upperValuesImpact);
	void nudgeWeights(fp* impacts);
	~FastLayer();
};