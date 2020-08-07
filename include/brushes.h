#pragma once
#include "graphics.h"
struct SolidColorBrush :public brush
{
	color c;
	SolidColorBrush(const color& c) :c(c) {}
	virtual color getColor(cvec2& pos) const override;
};

struct linkedBrush :brush
{
	//a brush linked to a graphicsObject
	//not linked to a brush in general, because a graphicsobject contains a width, height and depthbuffer
	const graphicsObject* g;
};

struct saturator : public linkedBrush
{
	fp addsaturation = 0.3;
	fp addvalue = 0.1;
	virtual color getColor(cvec2& pos) const override;

};
struct colorMultiplier : public linkedBrush
{
	vec3 multColors;
	virtual color getColor(cvec2& pos) const override;
};
struct alphaMask :public brush
{
	alphaMask(const brush* AlphaTex, const brush* basebrush);
	alphaMask();
	const brush* AlphaTex;
	const brush* basebrush;
	color getColor(const vec2& pos) const override;
};
struct colorMixer: public brush
{

	brush* topBrush;
	brush* bottomBrush;

	colorMixer(brush* topBrush, brush* bottomBrush):topBrush(topBrush), bottomBrush(bottomBrush){}
	color getColor(const vec2& pos) const override;
	static color getColor(color topColor, color bottomColor);
};
struct functionPointerBrush : public brush
{
	functionPointerBrush(color(*functionPointer)(const vec2& pos)):functionPointer(functionPointer){}
	color(*functionPointer)(const vec2& pos);
	color getColor(const vec2& pos) const override;
};
namespace brushes 
{
	extern brush* black;
	extern brush* white;
}