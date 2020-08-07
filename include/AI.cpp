#include "ai.h"

ai::ai(const aidata& data, FastLayer* input):calclayercount(data.calclayercount)
{
	FastLayer** layers = new FastLayer*[data.calclayercount];
	FastLayer** layerptr = layers;
	layers[0] = input;
	for (int i = 1; i < data.calclayercount; i++)
	{
		cint lastLayerCount = i == data.calclayercount - 1 ? data.neuroncount[i] - data.regressiveNeuronCount : 0;
		layers[i] = FastLayer::randomLayer(*layerptr++, data.neuroncount[i], lastLayerCount);
	}
	this->layers = layers;
	this->regressiveNeuronCount = data.regressiveNeuronCount;
}

fp ai::calculateLoss(cfp& difference, cfp& alpha)
{
	cfp abs2alpha = abs(2 - alpha);
	//(abs(2 - alpha) / alpha) * pow(math::squared(difference) / abs(2 - alpha) + 1,alpha * 0.5) - 1);
	//alpha replaced by a and difference by x:
	//((abs(2 - a) / a) * pow((x * x) / abs(2 - a) + 1,a * 0.5) - 1);
	if (abs2alpha == 0) //will otherwise cause NAN errors
	{
		return math::squared(difference) / 2;
	}
	else 
	{
		return (abs2alpha / alpha) *
			(
				pow(
					math::squared(difference) /
					abs2alpha
					+ 1,
					alpha * 0.5)
				- 1);
	}
}

ai::ai()
{
}

void ai::destruct()
{
	FastLayer** layerptr = layers;
	FastLayer** layerendptr = layers + calclayercount;
	while (layerptr < layerendptr)
	{
		delete* layerptr;
		layerptr++;
	}
	delete[] layers;
	while (regressionStates.size() > 0)
	{
		this->removeLastLayers();
	}
}

ai* ai::replicate(cfp& alpha)
{
	ai* child = new ai();
	child->regressiveNeuronCount = regressiveNeuronCount;
	child->MaxErrorTimeSteps = MaxErrorTimeSteps;
	child->calclayercount = calclayercount;
	child->layers = new FastLayer*[calclayercount];
	for (int i = 0; i < calclayercount; i++)
	{
		FastLayer* dadl = layers[i];
		FastLayer* childl = new FastLayer(i == 0 ? NULL : child->layers[i - 1], dadl->neuronCount, dadl, alpha);
		child->layers[i] = childl;
	}
	return child;
}

void ai::evaluate()
{
	FastLayer** layerptr = layers + 1;//1
	FastLayer** layerendptr = layers + calclayercount;//3
	//first, copy the last layer to the fist layer
	FastLayer* lastLayer = *(layerendptr - 1);
	FastLayer* firstLayer = *layers;
	cint regressionCount = lastLayer->neuronCount - lastLayer->lastLayerCount;
	std::copy(lastLayer->values + lastLayer->lastLayerCount, lastLayer->values + lastLayer->neuronCount,
		firstLayer->values + firstLayer->neuronCount - regressionCount);

	while (layerptr < layerendptr)//1, 2
	{
		(*layerptr)->Evaluate();
		layerptr++;
	}
}

void ai::Display(const graphicsObject* const graphics, cvec2& pos, cvec2& size, cvec2& blobsize)
{
	cvec2 halfBlobSize = blobsize * 0.5;
	cvec2 innerRect = (size - blobsize);
	cvec2 topleftBlobPos = pos + halfBlobSize;

	fp xStep = innerRect.x / (calclayercount - 1);
	for (int i = 0; i < calclayercount; i++)
	{
		fp xOff = topleftBlobPos.x + i * xStep;
		//draw a blob
		FastLayer* l = layers[i];
		fp yStep = innerRect.y / (l->neuronCount - 1);
		for (int j = 0; j < l->neuronCount; j++)
		{
			fp yOff = topleftBlobPos.y + j * yStep;
			//draw values
			cfp size = sigmoid01(l->values[j]);
			graphics->fillCircleCentered(xOff, yOff, size * blobsize.x, size * blobsize.y, SolidColorBrush(colorPalette::green));
		}
	}
}

void ai::DrawSelectedNeuron(const graphicsObject* const graphics, cvec2& pos, cvec2& size, cvec2& blobsize, cint& selectedLayer, cint& selectedNeuron)
{
	cvec2 halfBlobSize = blobsize * 0.5;
	cvec2 innerRect = (size - blobsize);
	cvec2 topleftBlobPos = pos + halfBlobSize;
	FastLayer* currentLayer = layers[selectedLayer];
	cvec2 selectedNeuronPos = GetNeuronPos(selectedLayer, selectedNeuron) * innerRect + topleftBlobPos;
	cfp treshold = 0.5;

	if (selectedLayer < calclayercount - 1)
	{
		FastLayer* upperLayer = layers[selectedLayer + 1];
		//get connection strength
		for (int i = 0; i < upperLayer->neuronCount; i++) 
		{
			cfp weight = upperLayer->weightmatrix[i + (selectedNeuron + 1) * upperLayer->neuronCount];
				graphics->DrawLine(selectedNeuronPos, GetNeuronPos(selectedLayer + 1, i) * innerRect + topleftBlobPos,
					getColor(weight));
		}
	} 
	if (selectedLayer > 0)
	{
		FastLayer* bottomLayer = layers[selectedLayer - 1];
		//get connection strength
		for (int i = 0; i < bottomLayer->neuronCount; i++)
		{
			cfp weight = currentLayer->weightmatrix[selectedNeuron + (i + 1) * currentLayer->neuronCount];
				graphics->DrawLine(selectedNeuronPos, GetNeuronPos(selectedLayer - 1, i) * innerRect + topleftBlobPos,
					getColor(weight));
		}
	}
}

void ai::DrawAllConnections(const graphicsObject* const graphics, cvec2& pos, cvec2& size, cvec2& blobsize)
{
	cvec2 halfBlobSize = blobsize * 0.5;
	cvec2 innerRect = (size - blobsize);
	cvec2 topleftBlobPos = pos + halfBlobSize;

	for (int k = 1; k < calclayercount; k++)
	{
		FastLayer* currentLayer = layers[k];
		FastLayer* bottomLayer = layers[k - 1];
		for (int j = 0; j < currentLayer->neuronCount; j++)
		{
			//get connection strength
			for (int i = 0; i < bottomLayer->neuronCount; i++)
			{
				cfp weight = currentLayer->weightmatrix[j + (i + 1) * currentLayer->neuronCount];
				graphics->DrawLine(GetNeuronPos(k, j) * innerRect + topleftBlobPos, GetNeuronPos(k - 1, i) * innerRect + topleftBlobPos,
					getColor(weight));

			}
		}
	}
}

void ai::removeLastLayers()
{
	//delete copied layers
	FastLayer** lastLayers = regressionStates[0];
	for (int i = 0; i < calclayercount; i++) 
	{
		delete lastLayers[i];
	}
	delete[] lastLayers;
	regressionStates.erase(regressionStates.begin());
}

void ai::memorizeCurrentLayers()
{
	FastLayer** copies = new FastLayer * [calclayercount];
	for(int i = 0; i < calclayercount;i++)
	{
		copies[i] = layers[i]->clone(i > 0 ? copies[i - 1] : nullptr);
	}
	this->regressionStates.push_back(copies);
}

void ai::manageregressionStates()
{
	if (regressionStates.size() >= MaxErrorTimeSteps)
	{
		//erase
		removeLastLayers();
	}
	//add
	memorizeCurrentLayers();
}

//THE ALPHA HAS TO BE ABOVE ZERO, ELSE THEY GET DUMBER
fp ai::Error(fp* expectedOutput, cfp& alpha)
{
	//first the output layer
	fp totalError = 0;
	FastLayer* lastLayer = layers[calclayercount - 1];
	fp* impact = new fp[lastLayer->neuronCount];
	for (int i = 0; i < lastLayer->lastLayerCount; i++)
	{
		fp cost = math::squared(expectedOutput[i] - lastLayer->values[i]);
		//the impact of the output on the cost to DECREASE the error
		//derivative of math::squared(val - expectedOutput)
		//multiply by alpha to reduce the impact of this learning session
		impact[i] = alpha * 2 * (expectedOutput[i] - lastLayer->values[i]);
		totalError += cost;
	}

	cint currentRegressionStateCount = regressionStates.size();
	
	fp** weightsImpact = new fp*[calclayercount - 1];
	bool end = true;//wether this is the end of the series of events that have caused this result
	//from current to the past, to trace back the errors
	//not int timeStep, because a int can't be less than zero
	for (int timeStep = currentRegressionStateCount; timeStep >= 0; timeStep--)
	{
		//not layer 0, because that is merely input or gets copied
		for (int i = calclayercount - 1; i > 0; i--)
		{
			FastLayer* currentLayer;
			if (timeStep == currentRegressionStateCount)//initialize
			{
				cint count = layers[i]->neuronCount * layers[i]->UpperLayerNeuronCountBias;
				weightsImpact[i - 1] = new fp[count];
				//fill the array
				std::fill(weightsImpact[i - 1], weightsImpact[i - 1] + count, 0);
				currentLayer = layers[i];
			}
			else 
			{
				currentLayer = regressionStates[timeStep][i];
			}
			fp* upperLayerValuesImpact;
			currentLayer->calculateImpact(impact, end, weightsImpact[i - 1], upperLayerValuesImpact);
			end = false;
			delete[] impact;
			if (i == 1) //first layer, cant apply impacts on the input values
			{
				impact = cut(upperLayerValuesImpact, (int)(layers[0]->neuronCount - regressiveNeuronCount), layers[0]->neuronCount);
				delete[] upperLayerValuesImpact;
			}
			else 
			{
				impact = upperLayerValuesImpact;
			}
		}
	}
	delete[] impact;
	//apply the weightImpacts
	for (int i = 1; i < calclayercount; i++)
	{
		layers[i]->nudgeWeights(weightsImpact[i - 1]);


		delete[] weightsImpact[i - 1];
	}
	delete[] weightsImpact;

	if (regressiveNeuronCount > 0) 
	{
		manageregressionStates();
	}
	return totalError / lastLayer->lastLayerCount;
}

vec2 ai::GetNeuronPos(cint& layer, cint& index)
{
	return vec2(layer / (calclayercount - 1.0), index / (layers[layer]->neuronCount - 1.0));
}
