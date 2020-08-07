#include "checkbox.h"

checkBox::checkBox(crectangle2i& rect) :Control(rect)
{

}

void checkBox::onClick()
{
	checked = !checked;
	onCheckedChanged();
}

void checkBox::onCheckedChanged()
{
	if (checkedChanged) 
	{
		checkedChanged();
	}
}


void checkBox::Draw(cvec2i& position, const graphicsObject& obj)
{
	Control::Draw(position, obj);
	if (checked)
	{
		//draw check mark on top
		drawCheckMark(position, obj);
	}
}

void checkBox::drawCheckMark(cvec2i& position, const graphicsObject& obj)
{
	//two lines
	obj.DrawLine(position + (vec2)rect.size * vec2(0.2,0.5), position + (vec2)rect.size * vec2(0.5, 0.2),checkMarkColor);
	obj.DrawLine(position + (vec2)rect.size * vec2(0.5, 0.2), position + (vec2)rect.size * vec2(1.1, 1.1), checkMarkColor);
}
