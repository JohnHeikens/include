#include "interaction.h"
//returns wether this key is pressed
//source:
//https://stackoverflow.com/questions/41606356/catch-ctrlalt-keys-in-wndproc
bool pressed(cushort& key)
{
	return GetKeyState(key) & 0x8000;
}

//process events
//return if to continue the program
bool DoEvents()
{
	MSG msg;
	BOOL result;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		result = ::GetMessage(&msg, NULL, 0, 0);
		if (result == 0) // WM_QUIT
		{
			::PostQuitMessage(msg.wParam);
			return false;
		}
		else if (result == -1)
		{
			return false;
			// Handle errors/exit application, etc.
		}
		else
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	return true;
}

bool GetClipboardText(std::wstring& result)
{
	if (!OpenClipboard(NULL))
		return false;

	result = StringToWString(std::string((const char*)GetClipboardData(CF_TEXT)));

	CloseClipboard();
	return true;
}
