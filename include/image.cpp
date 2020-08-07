/*#include "image.h"
inline Image::Image()
{

}
//COPIES THE POINTER, NOT THE COLORS
Image::Image(color* ptr, cint& w, cint& h, cint& channelcount)
{
	width = w;
	height = h;
	colors = ptr;
	this->ChannelCount = channelcount;
}

Image::Image(cint& w, cint& h)
{
	width = w;
	height = h;
	colors = new color[w * h];
}


inline color Image::GetPixel(int x, int y)
{
	return *(colors + x + y * width);
}

inline void Image::SetPixel(int x, int y, color color)
{
	*(colors + x + y * width) = color;
}

Image* Image::FromFile(std::wstring path, const bool flip)
{
	int w = 0, h = 0, channels = 0;
	std::vector<char> data = readBMP(path, w, h, channels);
	byte* c4 = new byte[w * h * 4];
	if (channels == 3)
	{
		InsertChannel((byte*)data.data(), (byte*)data.data() + w * h * 3, c4, 3, 4, 0xff);//fill alpha channel
	}
	else
	{
		memcpy(c4, data.data(), w * h * sizeof(color));
	}
	if (flip) //flip img vertically
	{
		int w4 = w * 4;
		byte* flipped = new byte[w4 * h];
		byte* ptr = flipped;
		byte* endptr = flipped + w4 * h;
		byte* srcptr = c4 + (h - 1) * w4;
		while (ptr < endptr)
		{
			memcpy(ptr, srcptr, w4);
			ptr += w4;
			srcptr -= w4;
		}
		delete[] c4;
		c4 = flipped;
	}
	return new Image((color*)c4, w, h, channels);
}

void Image::Save(std::wstring path, const bool& fullalpha)
{
	//write image to file
//source:
//https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
//padding MUST be included (round to 4 bytes)
	std::ofstream stream;//output file stream
	stream.open(path, std::ios::binary);
	if (stream.good()) {
		// mimeType = "image/bmp";
		stream.seekp(0, stream.beg);

		byte file[14]
		{
			'B','M', // magic
			0,0,0,0, // size in bytes
			0,0, // app data
			0,0, // app data
			40 + 14,0,0,0 // start of data offset
		};
		byte info[40]
		{
			40,0,0,0, // info hd size
			0,0,0,0, // width
			0,0,0,0, // heigth
			1,0, // number color planes
			ChannelCount * 8,0, // bits per pixel
			0,0,0,0, // compression is none
			0,0,0,0, // image bits size
			0xC3,0x03,0,0, // horz resolution in pixel / m
			0xC3,0x03,0,0, // vert resolutions (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
			0,0,0,0, // #colors in pallete
			0,0,0,0, // #important colors
		};
		int padSize, sizeData;
		if (ChannelCount == 3) 
		{
			padSize = //(4 - (Width * 3) % 4) % 4;
			(4 - 3 * width % 4) % 4;//how many bytes to add to be dividable by four?
			sizeData = width * height * 3 + height * padSize;
		}
		else 
		{
			padSize = 0;
			sizeData = width * height * 4;
		}
		int sizeAll = sizeData + sizeof(file) + sizeof(info);
		int strideX = ChannelCount;
		int strideY = width * strideX + padSize;

		file[2] = (byte)(sizeAll);
		file[3] = (byte)(sizeAll >> 8);
		file[4] = (byte)(sizeAll >> 16);
		file[5] = (byte)(sizeAll >> 24);

		info[4] = (byte)(width);
		info[5] = (byte)(width >> 8);
		info[6] = (byte)(width >> 16);
		info[7] = (byte)(width >> 24);

		info[8] = (byte)(height);
		info[9] = (byte)(height >> 8);
		info[10] = (byte)(height >> 16);
		info[11] = (byte)(height >> 24);

		info[20] = (byte)(sizeData);
		info[21] = (byte)(sizeData >> 8);
		info[22] = (byte)(sizeData >> 16);
		info[23] = (byte)(sizeData >> 24);

		stream.write((char*)file, sizeof(file));
		stream.write((char*)info, sizeof(info));

		byte pad[3] = { 0,0,0 };
		sizeof(byte);
		byte* ptr = (byte*)colors;
		//stream.write((char*)data, (strideY + padSize) * Height);
		for (int y = 0; y < height; y++)
		{
			byte* endPtr = ptr + width * strideX;
			byte* marginPtr = ptr + strideY;
			//switch r with b
			while (ptr < endPtr)
			{
				const char full = 0xff;
				stream.write((char*)ptr, sizeof(char));
				stream.write((char*)(ptr + 1), sizeof(char));
				stream.write((char*)(ptr + 2), sizeof(char));
				if (ChannelCount == 4)
				{
					stream.write(fullalpha ? (char*)&full : (char*)(ptr + 3), sizeof(char));
				}
				ptr += strideX;
			}
			//for (int i = 0; i < padSize; i++) 
			//{
			//	stream.write((char*)new char[1]{ 0 }, 1);
			//}
			while (ptr < marginPtr)
			{
				stream.write((char*)ptr++, 1);
			}
		}
		stream.close();
	}
}

color Image::getColor(const vec2& pos) const
{
	cint w = width;
	return *(colors + //index of current level
		(rendersettings::multsize ?
		((int)((rendersettings::Remaindering ? math::Remainder1(pos.x) : pos.x) * w) + //x
			(int)((rendersettings::Remaindering ? math::Remainder1(pos.y) : pos.y) * height) * w) : //y
			((int)pos.x + ((int)pos.y) * w)
			)
		);
}
*/
#include "image.h"
inline Image::Image()
{

}
//COPIES THE POINTER, NOT THE COLORS
Image::Image(color* ptr, int w, int h, int channelcount)
{
	width = w;
	height = h;
	colors = ptr;
	this->ChannelCount = channelcount;
}

Image::Image(int w, int h)
{
	width = w;
	height = h;
	colors = new color[w * h];
}

void Image::switchChannels(int channelIndex1, int channelIndex2)
{
	int size = width * height;
	color* endPtr = colors + size;
	for (color* ptr = colors; ptr < endPtr; ptr++)
	{
		std::swap(ptr->channels[channelIndex1], ptr->channels[channelIndex2]);
	}
}

void Image::Flip()
{
	color* swap0 = colors;
	//swap until the mid point, because else it would be swapped back
	//integer division, because the mid row does not have to be swapped
	color* swap0EndPtr = swap0 + (width / 2) * height;
	color* swap1 = swap0 + (height - 1) * width;
	while (swap0 < swap0EndPtr)
	{
		color* swap0X = swap0;
		color* swap1X = swap1;
		color* swap0XEndPtr = swap0X + width;
		while (swap0X < swap0XEndPtr)
		{
			std::swap(*swap0X, *swap1X);
			swap0X++;
			swap1X++;
		}
		swap0 += width;
		swap1 -= width;
	}
}

inline color Image::GetPixel(int x, int y)
{
	return *(colors + x + y * width);
}

inline void Image::SetPixel(int x, int y, color color)
{
	*(colors + x + y * width) = color;
}

Image* Image::FromFile(std::wstring path, const bool flip)
{
	Image* i = new Image();
	stbi_set_flip_vertically_on_load(flip);
	int w, h;
	i->colors = (color*)stbi_load(path.c_str(), &w, &h, &i->ChannelCount, colorchannels);
	i->width = w;
	i->height = h;
	//rgba to bgra 
	i->switchChannels(0, 2);
	return i;
}

void Image::Save(std::wstring path)
{
	std::wstring extension = path.substr(path.rfind(L'.') + 1);
	//bgra to rgba
	switchChannels(0, 2);
	if (extension == L"png")
	{
		stbi_write_png(path.c_str(), width, height, colorchannels, colors, 0);
	}
	else if (extension == L"bmp")
	{
		stbi_write_bmp(path.c_str(), width, height, colorchannels, colors);
	}
}

color Image::getColor(const vec2& pos) const
{
	cint w = width;
	return *(colors + //index of current level
		(rendersettings::multsize ?
		((int)((rendersettings::Remaindering ? math::Remainder1(pos.x) : pos.x) * w) + //x
			(int)((rendersettings::Remaindering ? math::Remainder1(pos.y) : pos.y) * height) * w) : //y
			((int)pos.x + ((int)pos.y) * w)
			)
		);
}
