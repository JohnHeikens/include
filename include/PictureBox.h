#include "Control.h"
#pragma once
struct PictureBox :public Control {
public:
	PictureBox(crectangle2i& rect, Image* image);
	Image* image = nullptr;
	virtual void Draw(cvec2i& position, const graphicsObject& obj) override;
};