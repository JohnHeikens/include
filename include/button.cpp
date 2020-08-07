#include "button.h"

button::button(crectangle2i& rect) :Control(rect)
{

}

void button::Draw(cvec2i& position, const graphicsObject& obj)
{
	miliseconds currentTime = getMiliseconds();
	fp part = (currentTime - lastClickTime) / 500.0;//500ms effect

	color c;
	if (part < 1) 
	{
		c = color::lerpcolor(clickColor, backGroundColor, part);
	}
	else 
	{
		c = backGroundColor;
	}
	obj.fillRectangle(rectangle2i(position + borderSize, rect.size - (borderSize + borderSize)), c);

	drawBorder(position, obj);
	drawText(position, obj);
}

void button::onClick()
{
	lastClickTime = getMiliseconds();
}
