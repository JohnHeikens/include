#include "errors.h"
std::wstring errormessages[errortypecount]//DO NOT FORGET THE COMMA (,)
{
	L"no operator found",
	L"calculation has a length of zero",
	L"no closing parenthesis \"}\" found",
	L"no closing square bracket \"]\" found",
	L"no closing brace \")\" found",
	L"something is typed after the closing bracket",
	L"this line is too short",
	L"could not find the function you specified",
	L"not all indexes of this array are filled",
	L"variable must be a modifyable l-value",
	L"could not find an appropriate function overload",
};
