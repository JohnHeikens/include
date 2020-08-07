#include "Control.h"
#include "button.h"
#pragma once
struct slider :public Control
{
	slider(crectangle2i& rect, cfp& minValue, cfp& maxValue, cfp& value);
	button* decreaseButton = nullptr;
	button* increaseButton = nullptr;
	button* dragButton = nullptr;
	void (*dragCompleted)() = nullptr;
	fp value = 0;
	fp minValue, maxValue;
	fp step = 1;

	void setValue(cfp& value);

	virtual void onMouseDown(cvec2i& position) override;
	virtual void onDragCompleted();
	virtual void drawCheckMark(cvec2i& position, const graphicsObject& obj);
	color checkMarkColor = colorPalette::white;
};