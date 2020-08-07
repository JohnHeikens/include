#pragma once
#include "GlobalFunctions.h"
enum filedialogmode { open, save };

WCHAR* getopenfilename(filedialogmode mode, const WCHAR* filter, const WCHAR* title);