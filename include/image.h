/*#pragma once
#include "Texture.h"
class Image:public Texture
{
public:
	Image();
	Image(color* ptr, cint& w, cint& h, cint& channelcount);
	Image(cint& w, cint& h);
	//the colors wont be deleted when the image is deleted.
	~Image();
	int ChannelCount;
	inline color GetPixel(int x, int y);
	inline void SetPixel(int x, int y, color color);
	static Image* FromFile(std::wstring path, const bool flip);
	void Save(std::wstring path, const bool& fullalpha = true);
	color getColor(const vec2& pos) const override;
private:

};
*/
#pragma once
#include "Texture.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION//needs to be BEFORE #include "ImageLoader.h" (ifndef)
#include "imageLoader.h"
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "imageWriter.h"
class Image :public Texture
{
public:
	Image();
	Image(color* ptr, int w, int h, int channelcount);
	Image(int w, int h);
	//the colors wont be deleted when the image is deleted.
	int ChannelCount;
	void switchChannels(int channelIndex1, int channelIndex2);
	void Flip();
	inline color GetPixel(int x, int y);
	inline void SetPixel(int x, int y, color color);
	static Image* FromFile(std::wstring path, const bool flip);
	void Save(std::wstring path);
	color getColor(const vec2& pos) const override;
private:

};