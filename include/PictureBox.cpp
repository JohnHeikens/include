#include "PictureBox.h"

PictureBox::PictureBox(crectangle2i& rect, Image* image):Control(rect)
{
	this->image = image;
}

void PictureBox::Draw(cvec2i& position, const graphicsObject& obj)
{
	obj.fillTexture(rectangle2(position, rect.size), *image);
}