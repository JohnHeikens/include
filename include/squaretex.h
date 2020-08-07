#pragma once
#include "Texture.h"
//max texture size: 64x64
constexpr int HeightIndexes[]{
	0b0,//1x1
	0b1,//2x2
	0b101,//4x4
	0b10101,//8x8
	0b1010101,//16x16
	0b101010101,//32x32
	0b10101010101,//64x64
	0b1010101010101,//128x128
	0b101010101010101,//256x256
	0b10101010101010101,//512x512
	0b1010101010101010101,//1024x1024
};
struct squaretex : public Texture
{

	int res;//the resolution steps of this texture
	int level;//the resolution level for getcolor
	void Load(color* colorptr, int w);
	squaretex();
	squaretex(color* colorptr, int w);
	color getColor(const vec2& pos) const;

};