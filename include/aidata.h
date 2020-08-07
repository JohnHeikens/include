#pragma once
//provides data from the ai
struct aidata
{
	//calclayercount: amount of layers that calculate from input neurons, including first and last layer
	//layer 0: input layer
	//layer (calclayercount - 1): outputlayer

	//regressionCount: the amount of neurons that store memory.
	//neuroncount: array with the amount of neurons per layer. MEMORY NEURONS HAVE TO BE INCLUDED!
	int calclayercount;
	int* neuroncount;
	int regressiveNeuronCount;
	inline aidata(const int calclayercount, int* const neuroncount, const int regressiveNeuronCount) :
		calclayercount(calclayercount), neuroncount(neuroncount), regressiveNeuronCount(regressiveNeuronCount) {  }
	inline aidata(){}
};