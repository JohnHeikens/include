#pragma once
#include "FastLayer.h"
#include "aidata.h"
#include "graphics.h"
#include "brushes.h"

struct ai
{
public:
	int calclayercount;
	int regressiveNeuronCount = 0;//the amount of neurons that will be copied to the 'start'
	int MaxErrorTimeSteps = 0;//the amount of timesteps that the error function will trace back
	FastLayer** layers;

	//0: least recent
	//size() - 1: most recent
	std::vector<FastLayer**> regressionStates = std::vector<FastLayer**>();;
	inline ai(const aidata& data) :ai(data, new FastLayer(nullptr, data.neuroncount[0], false)) {}
	ai(const aidata& data, FastLayer* input);

	//https://www.youtube.com/watch?v=BmNKbnF69eY
	static fp calculateLoss(cfp& difference, cfp& alpha);
	fp lossFunctionAlpha;

	ai();

	void destruct();

	ai* replicate(cfp& alpha);

	void evaluate();

	void removeLastLayers();
	void memorizeCurrentLayers();
	void manageregressionStates();
	fp Error(fp* expectedOutput, cfp& alpha);

	//positive = blue, negative red.
	inline color getColor(cfp& weight) 
	{
		return weight > 0 ?
			color(colorPalette::blue, byte(math::minimum(weight, (fp)1.0) * 0xff)) :
			color(colorPalette::red, byte(math::minimum(-weight, (fp)1.0) * 0xff));
	}
	//returns a vec2 ranging from 0 to 1
	vec2 GetNeuronPos(cint& layer, cint& index);

	void Display(const graphicsObject* const graphics, cvec2& pos, cvec2& size, cvec2& blobsize);
	void DrawSelectedNeuron(const graphicsObject* const graphics, cvec2& pos, cvec2& size, cvec2& blobsize, cint& selectedLayer, cint& selectedNeuron);
	void DrawAllConnections(const graphicsObject* const graphics, cvec2& pos, cvec2& size, cvec2& blobsize);
};
