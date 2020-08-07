#include "FastLayer.h"

fp(*activationFunction)(cfp& x) = leakyReLu6;
fp(*activationFunctionDer)(cfp& x) = leakyReLu6_d;

fp(*activationFunctionEnd)(cfp& x) = sigmoid01;
fp(*activationFunctionDerEnd)(cfp& x) = sigmoid01_d;

fp(*checkWeights)(cfp& x) = checkClamp;

FastLayer::FastLayer(FastLayer* const upperLayer, cint& neuronCount, FastLayer* const reproduceFrom, const fp& alpha) :FastLayer(upperLayer, neuronCount, reproduceFrom->lastLayerCount)
{
	if (upperLayer)
	{
		//generate weight matrix
		fp* ptr = weightmatrix;
		fp* reproduceFromPtr = reproduceFrom->weightmatrix;
		for (int j = 0; j < UpperLayerNeuronCountBias; j++)
		{
			for (int i = 0; i < neuronCount; i++, reproduceFromPtr++, ptr++)
			{
				//tweak the values from reproduceFrom
				*ptr = checkWeights(*reproduceFromPtr + randFp(-alpha, alpha));
			}
		}
	}
}

FastLayer::FastLayer(FastLayer* const upperLayer, cint& neuronCount, cint& lastLayerCount):
	neuronCount(neuronCount),UpperLayerNeuronCountBias(upperLayer? upperLayer->neuronCount + 1:0),upperLayer(upperLayer), lastLayerCount(lastLayerCount)
{
	//generate values
	values = new fp[neuronCount];
	totalSums = new fp[neuronCount];
	std::fill(values, values + neuronCount, 0);

	if (upperLayer) 
	{
		//generate weight matrix
		weightmatrix = new fp[UpperLayerNeuronCountBias * neuronCount];
	}
}

FastLayer* FastLayer::randomLayer(FastLayer* const upperLayer, cint& neuronCount, cint& lastLayerCount)
{
	FastLayer* l = new FastLayer(upperLayer,neuronCount,lastLayerCount);
	fp* ptr = l->weightmatrix;
	for (int j = 0; j < l->UpperLayerNeuronCountBias; j++)
	{
		for (int i = 0; i < neuronCount; i++, ptr++)
		{
			//ALWAYS FROM -1 TO 1, because else the neurons will have almost no effect at eachother
			*ptr = randFp(-1, 1);
		}
	}
	return l;
}

FastLayer* FastLayer::clone(FastLayer* const upperLayer)
{
	FastLayer* l = new FastLayer(upperLayer, neuronCount, lastLayerCount);
	//copy weightmatrix
	std::copy(weightmatrix, weightmatrix + neuronCount * UpperLayerNeuronCountBias, l->weightmatrix);
	//copy values
	std::copy(values, values + neuronCount, l->values);
	//copy totalSums
	std::copy(totalSums, totalSums + neuronCount, l->totalSums);
	return l;
}

void FastLayer::Evaluate()
{
	//multiply vector by the list
	
	fp* sumPtr = totalSums;
	cfp* biasWeightPtr = weightmatrix;
	cfp* valueWeightptr = biasWeightPtr + neuronCount;
	//copy weight values, because the bottom layer does not have to be multiplied; the bias is always one
	std::copy((cfp*)biasWeightPtr, valueWeightptr, sumPtr);

	//loop weights + upper neurons
	cfp* uppervalueptr = upperLayer->values;
	for (int j = 1; j < UpperLayerNeuronCountBias; j++, uppervalueptr++)//add weights multiplied by the values from the upper matrix
	{
		sumPtr = totalSums;
		//loop current neurons
		for (int i = 0; i < neuronCount; i++, sumPtr++, valueWeightptr++)
		{
			 *sumPtr += *valueWeightptr * *uppervalueptr;
		}
	}

	
	//apply a squashing function
	fp* valueptr = values;
	sumPtr = totalSums;
	for (int i = 0; i < neuronCount; i++, valueptr++, sumPtr++)//first the last layer neurons then the hidden layer neurons
	{
		*valueptr = i < lastLayerCount ? activationFunctionEnd(*sumPtr) : activationFunction(*sumPtr);
	}
}
//https://www.youtube.com/watch?v=tIeHLnjs5U8

//alpha can for example be 0.01;
//the outputs then will be nudged with a rate of one hundredth
//weightsImpact has to be an existing array
//it will be added
void FastLayer::calculateImpact(fp* curValuesImpact, cbool& end, fp*& weightsImpact, fp*& upperValuesImpact)
{
	cint startIndex = end ? 0 : lastLayerCount,
		endIndex = end ? lastLayerCount : neuronCount;

	fp(*currentActivationFunction_d)(cfp & x) = end ? activationFunctionDerEnd : activationFunctionDer;

	cint calculatingCount = endIndex - startIndex;
	
	upperValuesImpact = new fp[upperLayer->neuronCount];
	//copy existing values
	std::fill(upperValuesImpact, upperValuesImpact + upperLayer->neuronCount, 0);

	//cost = math::squared(output - desired output)
	//GOES WRONG IF STARTINDEX > 0!
	for (int relativeIndex = 0; relativeIndex < calculatingCount; relativeIndex++)
	{
		cint neuronIndex = relativeIndex + startIndex;
		cfp& val = values[neuronIndex];
		//a = b * c * d ->impact from b on a = c * d
		//compute nudge impacts
		//impact means how much this particular part of the formula caused the problem

		//the impact of the summed value on the cost
		cfp& summedImpact = currentActivationFunction_d(totalSums[neuronIndex]);

		cfp& summedTotalImpact = summedImpact * curValuesImpact[relativeIndex];
		//summedimpact = b + w1 * v1 + w2 * v2...
		for (int j = 0; j < UpperLayerNeuronCountBias; j++)
		{
			cfp weightImpact = j > 0 ? 
				//the impact of each weight on the summed value is just the current value
				upperLayer->values[j - 1] :
				//the impact of the bias on the summed value is just 1
				1;

			cfp weightTotalImpact = weightImpact * summedTotalImpact;


			//the impact of each current value on the summed value is just the weight
			cfp currentWeight = weightmatrix[neuronIndex + j * neuronCount];

			cfp valueImpact = currentWeight;

			cfp valueTotalImpact = valueImpact * summedTotalImpact;

			//adjust, so the cost will be lower next time
			//adjust the weights
			weightsImpact[neuronIndex + j * neuronCount] += weightTotalImpact;
			//adjust the values
			if (j > 0) {//the value of the bias can't be adjusted
				//is it -= or +=?
				
				//nudgeUpperLayerValues[j - 1] += valueTotalImpact;
				upperValuesImpact[j - 1] += valueTotalImpact;//not multiplying by alpha, because that will be done in that layer
			}
		}
	}
}

//add impacts to weights
void FastLayer::nudgeWeights(fp* impacts)
{
	cint count = neuronCount * UpperLayerNeuronCountBias;
	fp* weightEndPtr = weightmatrix + count;
	fp* impactPtr = impacts;
	for (fp* weightPtr = weightmatrix; weightPtr < weightEndPtr; weightPtr++, impactPtr++)
	{
		*weightPtr = checkWeights(*weightPtr + *impactPtr);
	}
}

FastLayer::~FastLayer()
{
	delete[] weightmatrix;
	delete[] values;
	delete[] totalSums;
}
