#include "gameForm.h"
gameForm* currentForm = nullptr;
gameForm::gameForm(crectangle2i& rect):form(rect)
{
	currentForm = this;
}


void gameForm::Draw(cvec2i& position, const graphicsObject& obj)
{
	//draw other elements on top
	DrawChildren(position, obj);
}
void gameForm::destruct()
{
	form::destruct();
}

