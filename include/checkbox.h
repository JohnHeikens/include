#include "Control.h"
#pragma once
struct checkBox :public Control
{
	checkBox(crectangle2i& rect);
	void (*checkedChanged)() = nullptr;
	bool checked = false;
	virtual void onClick() override;
	virtual void onCheckedChanged();
	virtual void Draw(cvec2i& position, const graphicsObject& obj) override;
	virtual void drawCheckMark(cvec2i& position, const graphicsObject& obj);
	color checkMarkColor = colorPalette::white;
};