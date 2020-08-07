#include "fontfamily.h"
#include "graphics.h"

struct letterDrawer : public linkedBrush
{
	const fontFamily* f = nullptr;
	rectangle2 letterRect = vec2();
	rectangle2 maskBrushRect = vec2();
	virtual color getColor(cvec2& pos) const override;
};

color letterDrawer::getColor(cvec2& pos) const
{
	color c = f->tex->getColor((pos - letterRect.pos00) / letterRect.size * maskBrushRect.size + maskBrushRect.pos00);
	return colorMixer::getColor(c, g->getColor(pos));
}

//the texture has to have a transparent background with black letters.
//Be careful: the actual texture will be modified!
fontFamily::fontFamily(Texture* texture, const bool flipRows)
{
	tex = texture;
	cvec2i texLetterSize = cvec2i(tex->width / asciiRowWidth);
	if (flipRows) 
	{
		for (int yRow0 = 0; yRow0 < asciiColumnHeight / 2; yRow0++)
		{
			int yRow1 = (asciiColumnHeight - yRow0 - 1);
			for(int aX = 0; aX < tex->width; aX++)
			{
				for (int yIndex = 0; yIndex < texLetterSize.y; yIndex++)
				{
					int aY0 = yRow0 * texLetterSize.y + yIndex;
					int aY1 = yRow1 * texLetterSize.y + yIndex;
					//swap colors
					std::swap(tex->colors[aX + aY0 * tex->width], tex->colors[aX + aY1 * tex->width]);
				}
			}
		}
	}
}

void fontFamily::DrawLetter(cletter& l, cvec2& position, cfp& letterSize, const graphicsObject& obj, const brush* b) const
{
	byte letterIndex = (byte)l;
	cvec2i asciiOffset = cvec2i(letterIndex % asciiRowWidth,//the x index of the letter image
		letterIndex / asciiRowWidth);//the y index of the letter image

	cvec2i texLetterSize = cvec2i(tex->width / asciiRowWidth);
	cvec2 texOffset = asciiOffset * texLetterSize;

	letterDrawer* drawer = new letterDrawer();
	drawer->maskBrushRect = rectangle2( texOffset,vec2(texLetterSize));
	drawer->letterRect = rectangle2(position, vec2(letterSize));
	drawer->f = this;
	drawer->g = &obj;
	obj.fillRectangleUnsafe(crectangle2i(position.x, position.y, letterSize, letterSize), drawer);

}

