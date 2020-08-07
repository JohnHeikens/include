#include "LayerNoiseSimplex.h"
constexpr fp MAX_NOISE_OUTPUT = 1;
constexpr fp NOISE_MULT = 1.0f / MAX_NOISE_OUTPUT;//multiply output by this to map from -1 to 1
//CAUTION! FREQUENCY HAS TO BE AN INVERSE!!!
LayerNoiseSimplex::LayerNoiseSimplex(uint seed, const fp* octaveWeights, cint octaveCount, fp min, fp max, fp frequency)
{
	fp TotalWeight = 0;
	BaseNoise = new SimplexNoise(seed);//initialize base noise
	this->octaveWeights = new fp[octaveCount];
	memcpy(this->octaveWeights, octaveWeights, octaveCount * sizeof(fp));
	//sum weights
	for (int i = 0; i < octaveCount; i++)
	{
		TotalWeight += this->octaveWeights[i];
	}

	//map weights
	fp mid = (min + max) * .5f;
	this->OutputPlus = mid;
	fp amplitude = max - mid;
	fp mult = amplitude * NOISE_MULT / TotalWeight;
	for (int i = 0; i < octaveCount; i++)
	{
		this->octaveWeights[i] *= mult;
	}
	this->octaveCount = octaveCount;
	this->frequency = frequency;
}


fp LayerNoiseSimplex::Evaluate1d(cfp& aX)
{
	fp val = OutputPlus;
	fp freq = frequency;
	for (int i = 0; i < octaveCount; i++)
	{
		val += octaveWeights[i] * BaseNoise->noise1(aX * freq);
		freq *= 2.0f;
	}
	if (val > 1)
	{
		return val + 1;
	}
	return val;
}
fp LayerNoiseSimplex::Evaluate2d(cfp& aX, cfp& aY)
{
	fp val = OutputPlus;
	fp freq = frequency;
	for (int i = 0; i < octaveCount; i++)
	{
		val += octaveWeights[i] * BaseNoise->noise2(aX * freq, aY * freq );
		freq *= 2.0f;
	}
	if (val > 1) 
	{
		return val + 1;
	}
	return val;
}

fp LayerNoiseSimplex::Evaluate3d(cfp& aX, cfp& aY, cfp& aZ)
{
	fp val = OutputPlus;
	fp freq = frequency;
	for (int i = 0; i < octaveCount; i++)
	{
		val += octaveWeights[i] * BaseNoise->noise3(aX * freq, aY * freq, aZ * freq);
		freq *= 2.0f;
	}
	return val;
}
fp LayerNoiseSimplex::Evaluate2d(cvec2& pos)
{
	return Evaluate2d(pos.x, pos.y);
}
fp LayerNoiseSimplex::Evaluate3d(cvec3& pos) 
{
	return Evaluate3d(pos.x, pos.y, pos.z);
}
fp* LayerNoiseSimplex::Evaluate1d(cint& X, cint& w)
{
	cint size = w;
	fp* OutputValues = new fp[size];
	fp* ptr = OutputValues;
	int EndX = X + w;
	for (int aX = X; aX < EndX; aX++)
	{
		fp val = OutputPlus;
		fp freq = frequency;
		for (int i = 0; i < octaveCount; i++)
		{
			val += octaveWeights[i] * BaseNoise->noise1(aX * freq);
			freq *= 2.0f;
		}
		*ptr++ = val;
	}
	return OutputValues;
}
fp* LayerNoiseSimplex::Evaluate2d(cint& X, cint& Y, cint& w, cint& l)
{
	cint size = w * l;
	fp* OutputValues = new fp[size];
	fp* ptr = OutputValues;
	int EndX = X + w;
	int EndY = Y + l;
	for (int aY = Y; aY < EndY; aY++)
	{
		for (int aX = X; aX < EndX; aX++)
		{
			fp val = OutputPlus;
			fp freq = frequency;
			for (int i = 0; i < octaveCount; i++)
			{
				val += octaveWeights[i] * BaseNoise->noise2(aX * freq, aY * freq);
				freq *= 2.0f;
			}
			*ptr++ = val;
		}
	}
	return OutputValues;
}

fp* LayerNoiseSimplex::Evaluate3d(cint& x, cint& y, cint& z, cint& w, cint& l, cint& h)
{
	cint size = w * l * h;
	fp* const OutputValues = new fp[size];
	fp* ptr = OutputValues;
	cint EndX = x + w;
	cint EndY = y + l;
	cint EndZ = z + h;
	for (int aZ = z; aZ < EndZ; aZ++)
	{
		for (int aY = y; aY < EndY; aY++)
		{
			for (int aX = x; aX < EndX; aX++)
			{
				fp val = OutputPlus;
				fp freq = frequency;
				for (int i = 0; i < octaveCount; i++)
				{
					val += octaveWeights[i] * BaseNoise->noise3(aX * freq, aY * freq, aZ * freq);
					freq *= 2.0f;
				}
				*ptr++ = val;
			}
		}
	}
	return OutputValues;
}

void LayerNoiseSimplex::destruct()
{
	delete BaseNoise;
	delete[] octaveWeights;

}
