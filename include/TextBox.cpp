#include "TextBox.h"

TextBox::TextBox(crectangle2i& rect):Control(rect)
{
}

void TextBox::Draw(cvec2i& position, const graphicsObject& obj)
{
	Control::Draw(position, obj);
	//draw cursor

}