#pragma once
#include "form.h"

struct application
{
	//data
	graphicsObject* graphics = new graphicsObject();
	form* mainForm;

	// Global Windows/Drawing variables
	HBITMAP hbmp = NULL;
	HWND hwnd = NULL;
	HDC hdcMem = NULL;
	// The window's DC
	HDC wndDC = NULL;
	HBITMAP hbmOld = NULL;

	POINT MousePos = POINT();
	bool lastKeyDown[0x100];
	//function pointer to initialize the form
	int run(form* (*initializeForm)(crectangle2i& rect), HINSTANCE hInstance);
	void processInput();
	void draw();
	void MakeSurface(HWND hwnd);
	static application* getApplicationConnected(HWND mainWindow);
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
