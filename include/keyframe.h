#pragma once
#include "GlobalFunctions.h"
template<typename t>
struct keyframe
{
	inline keyframe(cfp& location, const t& value)
	{
		this->location = location;
		this->value = value;
	}
	fp location;
	t value;
};
template<typename t>
struct transition
{
	inline transition(std::vector<keyframe<t>*>* keyframes)
	{
		this->keyframes = keyframes;
	}
	//has to be sorted from least to most
	std::vector<keyframe<t>*>* keyframes;
	inline t GetValue(cfp& location) const
	{
		const int KeyframeCount = keyframes->size();
		if ((*keyframes)[0]->location >= location) 
		{
			return (*keyframes)[0]->value;
		}
		for (int i = 1; i < keyframes->size();i++)
		{
			keyframe<t>* currentkeyframe = (*keyframes)[i];
			if (currentkeyframe->location >= location)
			{
				keyframe<t>* lastkeyframe = (*keyframes)[i - 1];
				//interpolate
				cfp weight = getw(lastkeyframe->location, currentkeyframe->location, location);
				return lerp(lastkeyframe->value, currentkeyframe->value, weight);
			}
		}
		return ((*keyframes)[KeyframeCount - 1])->value;
	}
};

