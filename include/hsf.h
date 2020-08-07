#include "GlobalFunctions.h"
#pragma once
struct hsfcontent 
{
public:
	std::wstring name;
	std::vector<std::wstring> data;
	int vertsize;//the vertical size of the array
};

//human-readable storage format
//lowercase
struct hsfreader
{
	std::list<hsfcontent> contents = std::list<hsfcontent>();
	hsfcontent getcontent(std::wstring name) 
	{
		for (hsfcontent c : contents) 
		{
			if (c.name == name)return c;
		}
		return hsfcontent();
	}
	static hsfreader fromfile(std::wstring path) 
	{
		hsfreader reader = hsfreader();
		std::wstring text = StringToWString(readalltext(path));
		makelowercase(text);
		std::vector<std::wstring> lines = split_string(text, L"\n");
		int start = 0;
		hsfcontent* content = NULL;
		for (int i = 0; i < lines.size(); i++)
		{
			std::wstring* str = &lines[i];
			int index = str->find(L':', 0);
			int commentindex = str->find(L"//", 0);
			if (commentindex != std::wstring::npos)
			{
				*str = str->substr(0, commentindex);
			}
			int sepindex = str->find(L':', 0);
			if (sepindex != std::wstring::npos) 
			{
				if (content) 
				{
					reader.contents.push_back(*content);
				}
				content = new hsfcontent();
				content->name = str->substr(0, sepindex);
				start = i + 1;
				*str = str->substr(sepindex + 1, str->length() - sepindex - 1);
			}
			if (content)
			{
				std::vector< std::wstring> contents = split_string(*str, L',', L"()\"\"{}[]");
				if (trim_copy(contents[0]) != L"")
				{
					content->vertsize++;
					for (std::wstring s : contents)
					{
						content->data.push_back(trim_copy(s));
					}
				}
			}
		}
		if (content)reader.contents.push_back(*content);
		return reader;
	}



};
