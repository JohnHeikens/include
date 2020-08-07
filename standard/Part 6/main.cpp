#include "main.h"

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	// Resize client
	application* app = new application();
	int success = app->run([](crectangle2i& rect) {return (form*)new gameForm(rect); }, hInstance);
	_CrtDumpMemoryLeaks();
	return success;
}