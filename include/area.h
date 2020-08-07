#include "operators.h"
#pragma once

const std::wstring skip = L"()[]\"\"";


struct area
{
	std::vector<action*> actions;
	std::vector<function*> functions;
	area* Parent;
	std::vector<variable*> variables;
	calculation* GetCalculation(std::wstring equation, variable** result);
	function* FindFunctionName(std::wstring name);
	variable* FindVariableName(std::wstring name);
	variable* GetVariable(std::wstring name);
	void ExecuteAllActions();
};
