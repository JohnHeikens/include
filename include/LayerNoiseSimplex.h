#pragma once
#include "SimplexNoise.h"//base noise
#include "vec3.h"
typedef unsigned int uint;
//#include <glad/glad.h>//typedefs
class LayerNoiseSimplex
{
public:
	LayerNoiseSimplex(uint seed, const fp* octaveWeights, cint octaveCount, const fp min, const fp max, const fp frequency);
	SimplexNoise* BaseNoise;
	int octaveCount;
	fp* octaveWeights;
	fp OutputPlus;//the addition to the output to map it between min and max
	fp frequency;//the multiplier for the input coordinates
	fp Evaluate1d(cfp& aX);
	fp Evaluate2d(cfp& aX, cfp& aY);
	fp Evaluate3d(cfp& aX, cfp& aY, cfp& aZ);
	fp Evaluate2d(cvec2& pos);
	fp Evaluate3d(cvec3& pos);
	fp* Evaluate1d(cint& X, cint& w);
	fp* Evaluate2d(cint& X, cint& Y, cint& w, cint& l);
	fp* Evaluate3d(cint& x, cint& y, cint& z, cint& w, cint& l, cint& h);
	
	void destruct();
};