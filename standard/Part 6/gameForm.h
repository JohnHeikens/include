#include "include.h"
#pragma once

struct gameForm :public form ,IDestructable
{
	gameForm(crectangle2i& rect);
	void Draw(cvec2i& position, const graphicsObject& obj) override;
	void destruct() override;
};