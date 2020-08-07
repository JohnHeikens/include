#include "font.h"
#include "graphics.h"
#include "brushes.h"
#include "interaction.h"
#include "fastlist.h"

struct theme
{
	theme(font* font = nullptr, int borderSize = 2, fp fontSize = 12, color backGroundColor = colorPalette::black, color borderColor = colorPalette::gray, const brush* textbrush = brushes::white) :
		font(font), borderSize(borderSize),fontSize(fontSize), backGroundColor(backGroundColor), borderColor(borderColor) {}
	int borderSize = 2;
	color backGroundColor = colorPalette::black;
	color borderColor = colorPalette::gray;
	font* font = nullptr;
	const brush* textbrush = brushes::white;
	fp fontSize = 12;
};

extern theme* defaultTheme;

#pragma once
struct Control:IDestructable
{
public:
	//pointer functions
	void (*draw)(cvec2i& position, const graphicsObject& obj) = nullptr;

	void (*keyDown)(cvk& keycode) = nullptr;
	void (*keyUp)(cvk& keycode) = nullptr;
	void (*keyPress)(cvk& keycode) = nullptr;
	
	void (*mouseDown)(cvec2i& position) = nullptr;
	void (*hover)(cvec2i& position) = nullptr;
	
	void (*click)() = nullptr;

	void (*focus)() = nullptr;
	void (*lostFocus)() = nullptr;

	Control(crectangle2i& rect);

	//function invokers
	//position includes the position of this element
	virtual void Draw(cvec2i& position, const graphicsObject& obj);
	virtual void drawBorder(cvec2i& position, const graphicsObject& obj);
	virtual void drawBackGround(cvec2i& position, const graphicsObject& obj);
	virtual void drawText(cvec2i& position, const graphicsObject& obj);
	virtual void DrawChildren(cvec2i& position, const graphicsObject& obj);
	virtual void onKeyDown(cvk& keycode);
	virtual void onKeyUp(cvk& keycode);
	virtual void onKeyPress(cvk& keycode);

	virtual void onHover(cvec2i& position);
	virtual void onMouseDown(cvec2i& position);
	virtual void onClick();
	//called when this control gains focus
	virtual void onFocus();
	//called when this control looses focus
	virtual void onLostFocus();

	virtual void destruct() override;

	//raycasts from top to bottom to check which child is hit
	Control* highestChild(cvec2i& pos);

	Control* parent = nullptr;

	bool visible = true;
	//if this control has focus, regardless of one of its child controls having focus
	bool focused = false;

	rectangle2i rect = rectangle2i();


	//pointer to the child that has the focus
	Control* focusedChild = nullptr;
	fastlist<Control*>* childs = nullptr;

	font* currentFont = nullptr;

	std::wstring text = L"";
	int borderSize = 2;

	color backGroundColor = colorPalette::black;
	color borderColor = colorPalette::gray;
};
//extern std::vector<Control*> Controls;

//void DrawAllControls(const graphicsObject& obj);
