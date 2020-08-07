#include "fontfamily.h"
struct font 
{
	fontFamily* family = nullptr;
	fp fontSize = 0x10;
	const brush* fontBrush = brushes::white;

	font(fontFamily* family, fp fontSize = 0x10, const brush* fontBrush = brushes::white) :
		family(family), fontSize(fontSize), fontBrush(fontBrush){}
	

	inline fp MeasureLetterWidth(const letter l) const
	{
		return fontSize;
	}
	inline int MeasureStringWidth(const std::wstring text) const
	{
		return fontSize * text.length();
	}
	void DrawString(const std::wstring text, crectangle2& rect, const graphicsObject& obj) const;
};