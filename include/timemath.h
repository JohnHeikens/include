#pragma once
#include "GlobalFunctions.h"

//typedefs
typedef long long miliseconds;//stores miliseconds
typedef __int64 microseconds;//stores microseconds
typedef long double seconds;

//the time at which the application booted
extern std::chrono::time_point<std::chrono::steady_clock> boottime;

inline uint GetSecond()
{
	return (uint)time(NULL);
}

//DONT CONVERT TO INT!
//HUGE NUMBERS
inline microseconds getmicroseconds()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
inline miliseconds getMiliseconds()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
inline seconds milisectosec(miliseconds ms) 
{
	return ms * 0.001;
}
inline seconds microsectosec(microseconds ms)
{
	return ms * 0.000001;
}
inline miliseconds getMiliseconds(seconds s)
{
	return (miliseconds)(s * 1000);
}

inline microseconds GetMicroSecondsSinceApplicationBoot()
{
	auto current_time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(current_time - boottime).count();
}
inline miliseconds GetMiliSecondsSinceApplicationBoot()
{
	auto current_time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(current_time - boottime).count();
}
inline seconds GetSecondsSinceApplicationBoot()
{
	return microsectosec(GetMicroSecondsSinceApplicationBoot());
}
