#include "font.h"

void font::DrawString(const std::wstring text, crectangle2& rect, const graphicsObject& obj) const
{
	if (text.length()) {
		const letter* textptr = text.c_str();
		const letter* textendptr = textptr + text.length();
		cvec2 topleft = vec2(rect.x, rect.y + rect.h - fontSize);
		vec2 off = topleft;//top left
		//color* colorptr = tex->colors;
		//combine both brushes
		while (textptr < textendptr)
		{
			if (*textptr == L'\n') 
			{
				off.x = topleft.x;
				off.y -= fontSize;
			}
			else 
			{
				byte b = (byte)*textptr;
				cint ix = b % 0x10;//the x index of the letter image
				cint iy = b / 0x10;//the y index of the letter image
				family->DrawLetter(*textptr, vec2(off.x, off.y), fontSize, obj, fontBrush);
				off.x += MeasureLetterWidth(*textptr);
			}
			textptr++;
		}
	}
}
