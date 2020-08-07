#include "Control.h"
#pragma once
struct TextBox :public Control {
public:
	TextBox(crectangle2i& rect);
	font* currentFont = nullptr;
	brush* textbrush;
	std::wstring text = L"";
	virtual void Draw(cvec2i& position, const graphicsObject& obj) override;
};