#include "brushes.h"

brush* brushes::black = new SolidColorBrush(colorPalette::black);
brush* brushes::white = new SolidColorBrush(colorPalette::white);


color SolidColorBrush::getColor(cvec2& pos) const
{
	return c;
}

color saturator::getColor(cvec2& pos) const
{
	const color original = g->getColor(pos);
	cvec3 rgbOriginal = colortovec(original);
	cvec3 hsv = rgb2hsv(rgbOriginal);
	cvec3 saturated = vec3(hsv.h, math::minimum(hsv.s + addsaturation, (fp)1.0), math::minimum(hsv.v + addvalue, (fp)1.0));
	cvec3 rgbSaturated = hsv2rgb(saturated);
	return vectocolor(rgbSaturated);
}
color colorMultiplier::getColor(cvec2& pos) const
{
	const color original = g->getColor(pos);

	return original * multColors;
}

alphaMask::alphaMask(const brush* AlphaTex, const brush* basebrush)
{
	this->AlphaTex = AlphaTex;
	this->basebrush = basebrush;
}

alphaMask::alphaMask()
{
}

color alphaMask::getColor(const vec2& pos) const
{
	const color c = basebrush->getColor(pos);
	const byte alpha = AlphaTex->getColor(pos).a;
	return color(c, alpha);
}

color functionPointerBrush::getColor(const vec2& pos) const
{
	return functionPointer(pos);
}

color colorMixer::getColor(const vec2& pos) const
{
	return getColor(topBrush->getColor(pos), bottomBrush->getColor(pos));
}
color colorMixer::getColor(color topColor, color bottomColor)
{
	//static functions
	if (topColor.a == 0xff || !rendersettings::checkopacity)
	{
		return topColor;
	}

	else
	{
		return color::transition(topColor, bottomColor);
	}
}