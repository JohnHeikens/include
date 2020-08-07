#pragma once
#include "brush.h"
#include "idestructable.h"
namespace rendersettings
{
	extern bool multsize;
	extern bool Remaindering;
}

constexpr int BinarySequence[]
{
	1,//1
	0b10,//2
	0b100,//4
	0b1000,//8
	0b10000,//16
	0b100000,//32
	0b1000000,//64
	0b10000000,//128
	0b100000000,//256
	0b1000000000,//512
	0b10000000000,//1024
};

//width and height MUST be a power of 2
//https://en.wikipedia.org/wiki/Texture_mapping
struct Texture:public brush,IDestructable
{
public:
	//variables
	//the size of this object
	int width = 0;
	int height = 0;
	//contains the colors of this object
	color* colors = nullptr;
	virtual color getColor(const vec2& pos) const override;
	static void Barycentric(const vec2& p, const vec2& a, const vec2& b, const vec2& c, fp& u, fp& v, fp& w);

	static mat3x3 GetBarycentricSet(const vec2& a, const vec2& b, const vec2& c);
	template<typename T>
	static inline fp interpolate(const T& u1, const T& v1, const T& w1, const vec2 p) 
	{
		return 1 - p.x - p.y * u1 + p.x * v1 + p.y * w1;
	}

	template<typename T>
	static inline void getcoordfunction(const T& u1, const T& v1, const T& w1, const mat3x3 matrix, T& begin, T& xplus, T& yplus)
	{
		vec2 coord00 = matrix.getstart();//x 0 y 0
		begin = (1 - coord00.x - coord00.y) * u1 + coord00.x * v1 + coord00.y * w1;
		vec2 coordxplus = matrix.getxstep();
		//the ones cancel eachother out
		xplus = (- coordxplus.x - coordxplus.y) * u1 + coordxplus.x * v1 + coordxplus.y * w1;
		vec2 coordyplus = matrix.getystep();
		yplus = ( - coordyplus.x - coordyplus.y) * u1 + coordyplus.x * v1 + coordyplus.y * w1;
	}
	virtual void destruct() override;
};
