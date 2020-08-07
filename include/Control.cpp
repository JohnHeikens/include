#include "Control.h"

theme* defaultTheme = nullptr;

void Control::Draw(cvec2i& position, const graphicsObject& obj)
{
	drawBackGround(position, obj);
	drawBorder(position, obj);
	drawText(position, obj);
	DrawChildren(position, obj);
}

void Control::drawBorder(cvec2i& position, const graphicsObject& obj)
{
	obj.fillRectangle(rectangle2i(position.x, position.y, rect.size.x, borderSize), borderColor);
	obj.fillRectangle(rectangle2i(position.x, position.y + rect.size.y - borderSize, rect.size.x, borderSize), borderColor);
	obj.fillRectangle(rectangle2i(position.x, position.y, borderSize, rect.size.y), borderColor);
	obj.fillRectangle(rectangle2i(position.x + rect.size.x - borderSize, position.y, borderSize, rect.size.y), borderColor);
}

void Control::drawBackGround(cvec2i& position, const graphicsObject& obj)
{
	obj.fillRectangle(rectangle2i(position + borderSize, rect.size - (borderSize + borderSize)), backGroundColor);
}

void Control::drawText(cvec2i& position, const graphicsObject& obj)
{
	currentFont->DrawString(text, rectangle2(rectangle2i(position,rect.size).expanded(-borderSize)), obj);
}

void Control::DrawChildren(cvec2i& position, const graphicsObject& obj)
{
	for (Control* element : *childs)
	{
		if (element->visible)
		{
			element->Draw(position + element->rect.pos00, obj);
		}
	}
}


Control::Control(crectangle2i& rect)
{
	this->rect = rect;
	childs = new fastlist<Control*>();
	this->currentFont = defaultTheme->font;
	this->borderSize = defaultTheme->borderSize;
	this->borderColor = defaultTheme->borderColor;
	this->backGroundColor = defaultTheme->backGroundColor;
}

void Control::onKeyDown(cvk& keycode)
{
	if (keyDown) {
		keyDown(keycode);
	}
	if (keycode == VK_SPACE || keycode == VK_RETURN)
	{
		onClick();
	}
	if (focusedChild) {
		focusedChild->onKeyDown(keycode);
	}
}

void Control::onKeyUp(cvk& keycode)
{
	if (keyUp) 
	{
		keyUp(keycode);
	}
	if (focusedChild) {
		focusedChild->onKeyUp(keycode);
	}
}

void Control::onKeyPress(cvk& keycode)
{
	if (keyPress) 
	{
		keyPress(keycode);
	}
	if (focusedChild) {
		focusedChild->onKeyPress(keycode);
	}
}
void Control::onHover(cvec2i& position)
{
	if (hover) 
	{
		hover(position);
	}
}
void Control::onMouseDown(cvec2i& position)
{
	Control* highest = highestChild(position);
	if (mouseDown) 
	{
		mouseDown(position);
	}
	onClick();
	if (highest != focusedChild)
	{
		if (focusedChild) 
		{
			focusedChild->onLostFocus();
		}
		focusedChild = highest;
		if (focusedChild)
		{
			focusedChild->onFocus();
		}
	}
	if (focusedChild)
	{
		focusedChild->onMouseDown(position - focusedChild->rect.pos00);
	}
}

void Control::onClick()
{
	if (click) 
	{
		click();
	}
}

void Control::onFocus()
{
	if(focus)
	{
		focus();
	}
}

void Control::onLostFocus()
{
	if (lostFocus) 
	{
		lostFocus();
	}
	if (focusedChild != nullptr) 
	{
		focusedChild->onLostFocus();
		focusedChild = nullptr;
	}
}

void Control::destruct()
{
	childs->destruct();
	delete childs;
}


Control* Control::highestChild(cvec2i& pos)
{
	//loop through childs from back to front
	for (int index = childs->size - 1; index >= 0; index--)
	{
		if ((((*childs)[index])->rect).contains(pos))
		{
			return (*childs)[index];
		}
	}
	return nullptr;
}