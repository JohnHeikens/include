#include "slider.h"

slider::slider(crectangle2i& rect, cfp& minValue, cfp& maxValue, cfp& value):Control(rect)
{
	cint buttonWidth = rect.h / math::goldenratio;
	increaseButton = new button(rectangle2i(rect.w - buttonWidth, 0, buttonWidth, rect.h));
	decreaseButton = new button(rectangle2i(0, 0, buttonWidth, rect.h));
	dragButton = new button(rectangle2i(buttonWidth, 0, buttonWidth, rect.h));
	childs->push_back(increaseButton);
	childs->push_back(decreaseButton);
	childs->push_back(dragButton);
	childs->update();

	this->minValue = minValue;
	this->maxValue = maxValue;
	setValue(value);
}

void slider::setValue(cfp& value)
{
	//calculate the x position of the slider
	cfp part = getw(minValue, maxValue, value);
	int room = rect.w - decreaseButton->rect.w - dragButton->rect.w - increaseButton->rect.w;
	dragButton->rect.x = decreaseButton->rect.w + part * room;
	this->value = value;
}

void slider::onMouseDown(cvec2i& position)
{
	Control* child = highestChild(position);
	if (!child || child == dragButton)
	{
		//the background was clicked
		int newDragButtonX = position.x - dragButton->rect.w / 2;
		newDragButtonX = clamp(newDragButtonX, increaseButton->rect.w, rect.w - increaseButton->rect.w - dragButton->rect.w);
		dragButton->rect.x = newDragButtonX;
		onDragCompleted();
	}
	else 
	{
		if (child == decreaseButton) 
		{
			value -= step;
		}
		else if (child == increaseButton)
		{
			value += step;
		}
	}
	Control::onMouseDown(position);
}

void slider::onDragCompleted()
{
	//calculate new value
	cfp part = dragButton->rect.x - decreaseButton->rect.w;
	cfp max = increaseButton->rect.x - dragButton->rect.w - decreaseButton->rect.w;
	value = mapValue(part, 0, max, minValue, maxValue);

	if (dragCompleted) 
	{
		dragCompleted();
	}
}
void slider::drawCheckMark(cvec2i& position, const graphicsObject& obj)
{
	//two lines
	obj.DrawLine(position + (vec2)rect.size * vec2(0.2, 0.5), position + (vec2)rect.size * vec2(0.5, 0.2), checkMarkColor);
	obj.DrawLine(position + (vec2)rect.size * vec2(0.5, 0.2), position + (vec2)rect.size * vec2(1.1, 1.1), checkMarkColor);
}
