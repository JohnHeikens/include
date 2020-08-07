#include "Control.h"
#pragma once
struct button :public Control 
{
	button(crectangle2i& rect);
	miliseconds lastClickTime = 0;
	color clickColor = colorPalette::white;
	virtual void onClick() override;
	virtual void Draw(cvec2i& position, const graphicsObject& obj) override;
};