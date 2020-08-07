#include "GlobalFunctions.h"
#pragma once
//https://en.wikipedia.org/wiki/Bracket
enum errortypes
{
	nooperator,
	calclengthzero,
	noclosingparenthese,//{}
	noclosingsquarebracket,//[]
	noclosingbrace,//()
	typedafterclosingbracket,
	linetooshort,
	couldnotfindfunction,
	notallindexesinitialized,
	mustbemodifyable,
	noavailableoverload,
	errortypecount,
};
extern std::wstring errormessages[errortypecount];
inline void ShowCompilerError(errortypes errortype, std::wstring code)
{
	if (MessageBox(NULL, (
		errormessages[errortype] + L"\ncode:\n" + code + L"\nerrorcode:" + std::to_wstring(errortype)
		+ L"\ncontinue compiling?").c_str(), L"Error", MB_YESNO) == IDYES)
	{

	}
	else
	{
		throw 0;
	}
}