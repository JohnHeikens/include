#include "graphics.h"
#include "brushes.h"
#pragma once

//0x10 letters in a row
cint asciiRowWidth = 0x10;
cint asciiColumnHeight = 0x10;
//the amount of letters in an ascii charachter set
cint asciiLetterCount = 0x100;

class fontFamily
{
public:
	fontFamily(Texture* texture, const bool flipRows = true);
	void DrawLetter(cletter& l, cvec2& position, cfp& letterSize, const graphicsObject& obj, const brush* b) const;
	Texture* tex;
	
private:

};

