#include "squaretex.h"
//COPIES THE COLORS, NOT THE POINTER
void squaretex::Load(color* colorptr, int w)
{
	res = 1;
	for (int i = 1; i < w; i *= 2)
	{
		res++;
	}
	level = res - 1;
	colors = (color*)calloc(HeightIndexes[res], sizeof(color));
	int ww = w * w;
	memcpy(colors + HeightIndexes[res - 1], colorptr, ww * sizeof(color));

	for (int i = res - 2; i >= 0; i--)
	{
		color* srcPtr = colors + HeightIndexes[i + 1];
		color* destPtr = colors + HeightIndexes[i];
		w = BinarySequence[i];
		int w2 = w + w;
		int w21 = w2 + 1;//x and y
		//average colors
		for (int y = 0; y < w; y++)
		{
			int y2 = y + y;
			color* srcY = srcPtr + y2 * w2;
			color* destY = destPtr + y * w;
			for (int x = 0; x < w; x++)
			{
				color* srcX = srcY + x + x;
				*(destY + x) = color::Average(
					//xy
					*srcX,			//00
					*(srcX + 1),		//10
					*(srcX + w2),	//01
					*(srcX + w21)	//11
				);
			}
		}
	}
}
squaretex::squaretex()
{
	colors = NULL;
}
squaretex::squaretex(color* colorptr, int w)
{
	Load(colorptr, w);
}

color squaretex::getColor(const vec2& pos) const
{
	cint w = BinarySequence[level];
	return *(colors + HeightIndexes[level] + //index of current level
		(rendersettings::multsize ?
		((int)((rendersettings::Remaindering ? math::Remainder1(pos.x) : pos.x) * w) + //x
			(int)((rendersettings::Remaindering ? math::Remainder1(pos.y) : pos.y) * w) * w) : //y
			((int)pos.x + ((int)pos.y) * w)
			)
		);
}