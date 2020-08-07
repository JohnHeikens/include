#pragma once

//includes
#include <Windows.h>//the basic include
#include <cmath>//all things that are relatable to math
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <codecvt>
#include <minmax.h>
#include <random>
#include <crtdbg.h>
#include "wtypes.h"
#include <thread>
#include <mmsystem.h>
#include <time.h>

#include <windowsx.h>

//https://docs.microsoft.com/en-us/cpp/parallel/amp/cpp-amp-overview?view=vs-2019
//#include <amp.h>//acceleration for parallel processing

//streams
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

//arrays
#include <array>
#include <iterator>
#include <vector>//vectors
#include <list>//lists
#include <string>
//#include <charconv> c++ 17 or more warnings

//defines
/*#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
*/

#define castout reinterpret_cast<const char*>//for writing to file streams
#define castin reinterpret_cast<char*>//for reading from file streams

#define RANDCOLOR (color(byte(rand()),byte(rand()),byte(rand())))

//not changing types, but make it easier to type every time.
typedef long double fp;//a floating point precision-value, precision can be fp(4 bytes), fp(8 bytes, fp 4 = 8), or long fp(16 bytes).
typedef long long ll;
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef wchar_t letter;//unicode charachter
//virtual keycode
typedef byte vk;

typedef const ll cll;
typedef const int cint;
typedef const byte cbyte;
typedef const uint cuint;
typedef const bool cbool;
typedef const int cint;
typedef const ushort cushort;
//const floating point
typedef const fp cfp;
typedef const letter cletter;
typedef std::vector<std::wstring> wstringcontainer;
typedef std::vector<std::string> stringContainer;
//const virtual keycode
typedef const vk cvk;

//stored in .cpp file:
RECT GetMonitorRect();
RECT GetDesktopRect();
void GetMonitorResolution(int& horizontal, int& vertical);
byte* ResizeArray(byte* ptr, cint newsize);

void writeStringToStream(std::ofstream& const stream, const std::string str);
std::string readStringFromStream(std::ifstream& stream);
void writeWStringToStream(std::ofstream& const stream, const std::wstring str);
std::wstring readWStringFromStream(std::ifstream& stream);

//void writeWStringToStream(std::ofstream& stream, const std::string str);
//std::string readStringFromStream(std::ifstream& stream);
std::vector<char> readBMP(const std::wstring& file, int& width, int& height, int& channels);
void InsertChannel(byte* arrayPtr, byte* ArrayEndPtr, byte* newarrayPtr, int start, int stride, byte value);

//simulates 1 + 2 + 3 + 4 + 5...
inline fp calculateIterativeAddition(cfp& iterationCount)
{
	return iterationCount * (iterationCount + 1) * 0.5;
}

inline bool checkFileExists(std::wstring fileName) 
{
	std::ifstream infile(fileName);
	return infile.good();
}
inline bool checkFileExists(std::string fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

//writes to output window
inline void output(std::wstring s)
{
	OutputDebugString(s.c_str());
}

//returns a when w = 0
//returns b when w = 1
//interpolates betwee
template<typename t>
	inline t lerp(const t a, const t b, const fp w)
	{
		return a + (b - a) * w;
	}

	//a + (b - a) * w = c
	//(b - a) * w = c - a
	//w = (c - a)/(b - a)
	template<typename t>
	inline fp getw(const t a, const t b, const t c)
	{
		return (c - a) / (b - a);
	}
constexpr int ByteToBits = 8;//there are 8 bits in a byte
constexpr fp bytemult0to1 = 1.0 / 0xff;
constexpr byte bytemax = (byte)0xff;
constexpr int RandMax1 = RAND_MAX + 1;

inline fp randFp()
{
	return (fp)rand() / RAND_MAX;
}
inline fp randFp(cfp& max)
{
	return randFp() * max;
}
inline fp randFp(cfp& min, cfp& max)
{
	return lerp(min, max, randFp());
}

template<typename t>
int indexof(std::vector<t> v, t value) 
{
	return std::distance(v.begin(), std::find(v.begin(), v.end(), value));
}

int Math_Fix(fp f);
int FloorToBase(int val, int base);
inline fp mapValue(const fp& in, const fp& imin, const fp& imax, const fp& omin, const fp& omax)
{
	const fp mult = (omax - omin) / (imax - imin);
	fp mapped = (in - imin) * mult + omin;
	return mapped;
}
//to limit a value between bounds
template<typename t>
inline fp clamp(const t& value, const t& min, const t& max)
{
	return value < min ? min : value > max ? max : value;
}
inline int GetSign(cint& val) 
{
	return val > 0 ? 1 : val < 0 ? -1 : 0;
}
//https://stackoverflow.com/questions/2622441/c-integer-floor-function
inline int FloorDiv(int a, int b) 
{
	if (a < 0 || b < 0) 
	{
		ldiv_t res = ldiv(a, b);
		return (res.rem) ? res.quot - 1
			: res.quot;
	}
	else 
	{
		return a / b;
	}
}

//copies a piece out of an old array into a new array
template<typename t>
inline t* cut(t* const& arr, cint& start, cint& end)
{
	if (end - start == 0) 
	{
		return nullptr;
	}
	t* newArray = new t[end - start];
	std::copy(arr + start, arr + end, newArray);
	return newArray;
}

//litteraly copies chars to letters
//https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
inline std::wstring StringToWString(const std::string& s)
{
	//corrupts memory
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	return converter.from_bytes(s);//DONT EVER USE converterX.from_bytes(s) it will cost gigabytes of memory!
	//std::wstring wsTmp(s.begin(), s.end());
	//return wsTmp;
}
//litteraly copies letters to chars
inline std::string WStringToString(const std::wstring& s)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(s);
	//std::string sTmp(s.begin(), s.end());
	//return sTmp;
}

//reads an entire file into a string.
//source
//https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
inline std::stringstream readtostringstream(std::wstring path)
{
	std::ifstream t(path);
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer;
}
inline std::string readalltext(std::wstring path)
{
	return readtostringstream(path).str();
}
inline void writealltext(std::wstring path, std::string text)
{
	std::ofstream s(path);
	s << text;
	s.close();
}
//https://www.codespeedy.com/fetch-a-random-line-from-a-text-file-in-cpp/
inline std::vector<std::string> readAllLines(std::wstring path)
{
	std::string line;
	std::vector<std::string> lines;
	//input file stream
	std::ifstream file(path);

	//count number of total lines in the file and store the lines in the string vector
	int total_lines = 0;
	while (std::getline(file, line))
	{
		total_lines++;
		lines.push_back(line);
	}
	return lines;
}
inline fp PowIntSimple(const fp value, cint power) {
	fp p;
	if (power > 0)
	{
		p = value;
		for (int i = 1; i < power; i++)
			p *= value;
	}
	else if (power < 0)
	{
		p = 1.0 / value;
		for (int i = 1; i < power; i++)
			p /= value;
	}
	else 
	{
		return 1;
	}
	return p;
}

inline std::wstring towstring(cfp value, cint digits)
{
	std::wstringstream ss;
	ss << std::fixed << std::setprecision(digits) << value;
	return ss.str();
};

//https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start (in place)
static inline void ltrim(std::wstring& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
static inline void rtrim(std::wstring& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::wstring& s) 
{
	ltrim(s);
	rtrim(s);
}

// trim from start (copying)
static inline std::wstring ltrim_copy(std::wstring s) {
	ltrim(s);
	return s;
}

// trim from end (copying)
static inline std::wstring rtrim_copy(std::wstring s) {
	rtrim(s);
	return s;
}

// trim from both ends (copying)
static inline std::wstring trim_copy(std::wstring s) {
	trim(s);
	return s;
}

//DO NOT NAME EQUAL
inline bool substrcomp(std::wstring str, std::wstring substring, int offset = 0)
{
	return str.substr(offset, substring.size()) == substring;
}

inline int Find(std::wstring str, int offset, std::wstring seekfor, std::wstring skip)
{
	while (offset < str.size())
	{
		if (substrcomp(str, seekfor, offset))
		{
			return offset;
		}
		else
		{
			int index = skip.find(str[offset]);
			if (index != -1)
			{
				offset = Find(str, offset + 1, std::wstring() + skip[index + 1], skip);
			}
		}
		offset++;
	}
	return -1;
}
inline std::vector<std::wstring> split_string(const std::wstring& str, const letter& delimiter, const std::wstring& skip)
{
	std::vector<std::wstring> strings;
	int start = 0;
	for (int i = 0; i < str.size(); i++) 
	{
		const letter c = str[i];
		if (c == delimiter)
		{
			strings.push_back(str.substr(start, i - start));
			start = i + 1;
		}
		else {
			int index = skip.find(c, 0);
			if (index != std::wstring::npos)
			{
				i = Find(str, i + 1, std::wstring{ skip[index + 1] }, skip);
				if (i == std::wstring::npos) break;
			}
		}
	}
	// To get the last substring (or only, if delimiter is not found)
	strings.push_back(str.substr(start));
	return strings;
}
inline std::vector<std::wstring> split_string(const std::wstring& str, const std::wstring& delimiter, const std::wstring& skip)
{
	std::vector<std::wstring> strings;
	int start = 0;
	for (int i = 0; i < str.size(); i++)
	{
		const std::wstring s = str.substr(i, delimiter.size());
		if (s == delimiter)
		{
			strings.push_back(str.substr(start, i - start));
			start = i + delimiter.size();
		}
		else {
			int index = skip.find(str[i], 0);
			if (index != std::wstring::npos)
			{
				i = Find(str, i + 1, std::wstring(1,skip[index + 1]), skip);
			}
		}
	}
	// To get the last substring (or only, if delimiter is not found)
	strings.push_back(str.substr(start));
	return strings;
}

inline std::vector<std::wstring> split_string(const std::wstring& str, const std::wstring& delimiter)
{
	std::vector<std::wstring> strings;

	std::wstring::size_type pos = 0;
	std::wstring::size_type prev = 0;
	while ((pos = str.find(delimiter, prev)) != std::wstring::npos)
	{
		strings.push_back(str.substr(prev, pos - prev));
		prev = pos + delimiter.size();
	}

	// To get the last substring (or only, if delimiter is not found)
	strings.push_back(str.substr(prev));

	return strings;
}

inline std::wstring replace(std::wstring str, std::wstring from, std::wstring to)
{
	int lastindex = 0;
	while (true)
	{
		cint newindex = str.find(from, lastindex);
		if (newindex == std::wstring::npos)
		{
			return str;
		}
		else 
		{
			str = str.substr(0, newindex) + to + str.substr(newindex + from.length());
			lastindex = newindex + to.length();
		}
	}
}
inline int FindLine(wstringcontainer lines, int offset, std::wstring seekfor)
{
	while (offset < lines.size())
	{
		if (lines[offset] == seekfor)
			return offset;
		offset++;
	}
	return std::wstring::npos;
}
inline int FindLine(wstringcontainer lines, int offset, std::wstring seekfor, wstringcontainer skip)
{
	while (offset < lines.size())
	{
		if (lines[offset] == seekfor)
		{
			return offset;
		}
		else 
		{
			int index = FindLine(skip, 0, lines[offset]);
			if (index != std::wstring::npos) 
			{
				offset = FindLine(lines, offset + 1, skip[index + 1],skip);
			}
		}
		offset++;
	}
	return -1;
}


//https://stackoverflow.com/questions/8518743/get-directory-from-file-path-c/14631366
//get the directory path without slash
inline std::wstring getdirectorypath(const std::wstring& fname)
{
	int pos = fname.find_last_of(L"\\/");
	return (std::wstring::npos == pos)
		? L""
		: fname.substr(0, pos);
}

bool strtol(const std::wstring line, long& number, int radix);

//brga structure
struct color
{
	union {
		struct {
			byte b;//the blue component
			byte g;//the green component
			byte r;//the red component
			byte a;//the alpha component
		};
		int val;//used for fast memory movement
		byte channels[4];
	};
public:
	color(const color& c, cbyte& a) :a(a),r(c.r),g(c.g),b(c.b){}
	color(cbyte& a, cbyte& r, cbyte& g, cbyte& b) :a(a), r(r), g(g), b(b) {}
	color(cint& a, cint& r, cint& g, cint& b) :a(a), r(r), g(g), b(b) {}
	color(cbyte& r, cbyte& g, cbyte& b) :a(0xff), r(r), g(g), b(b) {}
	color(cint& r, cint& g, cint& b) :a(0xff), r(r), g(g), b(b) {}
	//WARNING! MULTIPLIED BY 0XFF
	color(cfp& r, cfp& g, cfp& b) :a(0xff), r((byte)(r * 0xff)), g((byte)(g * 0xff)), b((byte)(b * 0xff)) {}
	//WARNING! MULTIPLIED BY 0XFF
	color(cfp& a, cfp& r, cfp& g, cfp& b) :
		a((byte)(a * 0xff)), r((byte)(r * 0xff)), g((byte)(g * 0xff)), b((byte)(b * 0xff)) {}
	color(cbyte& a, cbyte& grayscale) :a(a), r(grayscale), g(grayscale), b(grayscale) {}
	color(cbyte& grayscale) :a(0xff), r(grayscale), g(grayscale), b(grayscale) {}
	color() :val(0) {}
	static inline color RandomRGB()
	{
		//https://stackoverflow.com/questions/22883840/c-get-random-number-from-0-to-max-long-long-integer/22883968
		return color(rand() % 0x100, rand() % 0x100, rand() % 0x100);//cast directly to color, copy bytes
	}
	//divide brightness over r, g and b
	//the output color will have r + g + b = brightness
	static inline color FromBrightness(int brightness) 
	{
		color clr = color();
		int a, b, c;
		int rest = brightness;
		int minrest = min(rest + 1, 0x100);
		int minimum = max(rest - 0xff * 2, 0);
		a = rand() % (minrest - minimum) + minimum;
		rest -= a;//calculate how much brightness is left
		minrest = min(rest + 1, 0x100);
		minimum = max(rest - 0xff, 0);
		b = rand() % (minrest - minimum) + minimum;
		rest -= b;
		c = rest;
		//shuffle a, b and c
		int shuffleval = rand();

		if (shuffleval & 0b1)
		{
			std::swap(a, b);
		}
		if (shuffleval & 0b01)
		{
			std::swap(b, c);
		}
		if (shuffleval & 0b001)
		{
			std::swap(a, c);
		}
		return color(a, b, c);
	}
	static inline color FromHex(cuint hex) 
	{
		return color((byte)(hex / 0x10000), (byte)(hex / 0x100), (byte)hex);
	}
	static inline color fromwstring(std::wstring str) 
	{
		color clr = color();
		clr.a = 0xff;
		if (str[0] == L'#') 
		{
			long l;
			if (strtol(str.substr(1, 2), l, 16)) 
			{
				clr.r = (byte)l;
				if (strtol(str.substr(3, 2), l, 16))
				{
					clr.g = (byte)l;
					if (strtol(str.substr(5, 2), l, 16))
					{
						clr.b = (byte)l;
					}
				}
			}
		}
		return clr;
	}
	//returns the average of 4 colors.
	static inline color Average(const color& c1, const color& c2, const color& c3, const color& c4)
	{
		return color(
			((int)c1.a + c2.a + c3.a + c4.a) / 4,
			((int)c1.r + c2.r + c3.r + c4.r) / 4,
			((int)c1.g + c2.g + c3.g + c4.g) / 4,
			((int)c1.b + c2.b + c3.b + c4.b) / 4
		);
	}

	//returns c1 on 0 and c2 on 1 and lerps between
	static inline color lerpcolor(const color& c1, const color& c2, const fp& weight)
	{
		return color(
			(byte)(c1.r + weight * (c2.r - c1.r)),
			(byte)(c1.g + weight * (c2.g - c1.g)),
			(byte)(c1.b + weight * (c2.b - c1.b))
		);
	}
	//returns the 'top view' of the colors, above eachother
	static inline color transition(const color& topcolor, const color& bottomcolor)
	{
		cint mergeda = 0xff - (((0xff - bottomcolor.a) * (0xff - topcolor.a))/0xff);
		cint mergedr = bottomcolor.r + ((topcolor.a * ((int)topcolor.r - bottomcolor.r)) / 0xff);
		cint mergedg = bottomcolor.g + ((topcolor.a * ((int)topcolor.g - bottomcolor.g)) / 0xff);
		cint mergedb = bottomcolor.b + ((topcolor.a * ((int)topcolor.b - bottomcolor.b)) / 0xff);
		const color c = color(
			mergeda,
			mergedr,
			mergedg,
			mergedb
		);
		return c;
	}
};

//all basic colors
namespace colorPalette
{
	const color transparent = color((byte)0, (byte)0);
	const color black = color((byte)0, (byte)0, (byte)0);
	const color red = color(bytemax, (byte)0, (byte)0);
	const color green = color((byte)0, bytemax, (byte)0);
	const color blue = color((byte)0, (byte)0, bytemax);
	const color yellow = color(bytemax, bytemax, (byte)0);
	const color purple = color(bytemax, (byte)0, bytemax);
	const color aqua = color((byte)0, bytemax, bytemax);
	const color white = color(bytemax, bytemax, bytemax);
	const color gray = color((byte)0x80, (byte)0x80, (byte)0x80);
	const color brown = color((byte)0x80, (byte)0x40, (byte)0x40);
}

constexpr int colorchannels = 4, colorbits = colorchannels * ByteToBits;
void makelowercase(std::wstring& s);
color operator * (color c, fp multiplier);
bool strtod(const std::wstring line, fp& number);
bool operator == (const color& a, const color& b);


//global functions:


//CAUTION: IT WILL GO WRONG AT VALUES ABOVE (INT_MAX / 2)
//https://www.codeproject.com/Tips/700780/Fast-floor-ceiling-functions
inline int fastfloor(const fp& value) 
{
	/*
	static cint intmax2 = INT_MAX / 2;
	static const double intmaxdouble = intmax2;
	return (int)(value + intmaxdouble) - intmax2;*/
	return (int)floor(value);
}
inline int fastceil(const fp& value)
{
	static cint intmax2 = INT_MAX / 2;
	static const fp intmaxdouble = intmax2;
	return intmax2 - (int)(intmaxdouble - value);
}
uint* getplaneindices(int planecount);

HWND MakeWindow(RECT windowrect, HINSTANCE hInstance, WNDPROC proc, void* userData = nullptr, bool fullScreen = false);
namespace math
{
	constexpr fp PI = 3.14159265359;
	constexpr fp PI2 = PI + PI;//a whole circle
	constexpr fp goldenratio = 1.618033988749894848204586834;
	constexpr fp degtorad = PI / 180;
	constexpr fp RadToDeg = 180 / PI;
	constexpr fp fpepsilon = 0.0001;
	inline fp Log(cfp value, cfp base)
	{
		return std::log(value) / std::log(base);
	}
	template<typename t>
	inline t squared(const t& value)
	{
		return value * value;
	}

	//floors a value to a unit (floor(5,2) will return 4)
	template<typename t>
	inline t floor(const t value, const t unit)
	{
		cfp divided = (fp)value / unit;//divide by the value
		cint toint = (int)std::floor(divided);//round towards zero
		return toint * unit;//multiply back, so we basically rounded it down in units of [unit]
	}
	//floors a value to a given amount of digits
	inline fp floor(cfp value, cint digits)
	{
		cfp difference = PowIntSimple(10, digits);
		return floor(value, difference);
	}
	//floors a value to a unit and substracts the input from the result (mod(5,2) will return 1)
	template<typename t>
	inline t mod(const t value, const t unit)
	{
		return value - floor(value, unit);//return difference
	}
	template<typename t>
	inline t maximum(const t& left, const t& right)
	{
		return left > right ? left : right;
	}
	template<typename t>
	inline t minimum(const t& left, const t& right)
	{
		return left < right ? left : right;
	}


	//same slope = 0
	inline fp calculateAngle(cfp& slope0, cfp& slope1)
	{
		return abs(atan((slope1 - slope0) / (1 + slope1 * slope0)));
	}
	inline fp Remainder1(const fp& value)
	{
		return value - std::floor(value);
	}
	template<typename t>
	inline int sign(const t& value)
	{
		return value > 0 ? 1 : value < 0 ? -1 : 0;
	}
}