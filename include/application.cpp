#include "application.h"

int application::run(form* (*initializeForm)(crectangle2i& rect), HINSTANCE hInstance)
{
	hwnd = MakeWindow(GetMonitorRect(), hInstance, WndProc, this, false);
	rendersettings::checkopacity = true;
	srand(getmicroseconds());

	fontFamily* family = new fontFamily(Image::FromFile(L"data\\ascii.png", true));
	defaultTheme = new theme(new font(family));

	//initialize form
	mainForm = initializeForm(graphics->getClientRect());

	while (DoEvents())//next frame
	{
		processInput();//process events from user
		// Do stuff with graphics->colors
		draw();
		// Draw graphics->colors to window
		BitBlt(wndDC, 0, 0, graphics->width, graphics->height, hdcMem, 0, 0, SRCCOPY);
	}
	mainForm->destruct();
	delete mainForm;
	delete graphics;
	delete brushes::black;
	delete brushes::white;
	family->tex->destruct();
	delete family->tex;
	delete family;
	delete defaultTheme;
	_CrtDumpMemoryLeaks();
	return 0;
}

LRESULT CALLBACK WndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{

	application* app = application::getApplicationConnected(hwnd);
	switch (msg) {
	case WM_CREATE:
	{
		//https://social.msdn.microsoft.com/Forums/vstudio/en-US/b9ec34b5-827b-4357-8344-939baf284f46/win32-about-createwindowex-and-wndproc-pointer-relations?forum=vclanguage
		LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
		app = (application*)lpcs->lpCreateParams;

		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)app);//connect the window to this application
		app->MakeSurface(hwnd);
	}
	break;
	case WM_MOUSEMOVE:
	{

	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		// Draw graphics->colors to window when window needs repainting
		BitBlt(hdc, 0, 0, app->graphics->width, app->graphics->height, app->hdcMem, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
	}
	break;
	case WM_DESTROY:
	{
		SelectObject(app->hdcMem, app->hbmOld);
		DeleteDC(app->wndDC);
		PostQuitMessage(0);
	}
	break;
	case WM_KEYDOWN:
	{
		if (!app->lastKeyDown[wParam])
		{
			app->mainForm->onKeyDown(wParam);
		}
		app->lastKeyDown[wParam] = true;
	}
	break;
	case WM_KEYUP:
	{
		app->mainForm->onKeyUp(wParam);
		app->lastKeyDown[wParam] = false;
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (!app->lastKeyDown[VK_LBUTTON])
		{
			app->mainForm->onMouseDown(cvec2i(app->MousePos.x, app->MousePos.y));
			app->lastKeyDown[VK_LBUTTON] = true;
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		//f->onMouseUp(cvec2i(MousePos.x, MousePos.y));
		app->lastKeyDown[VK_LBUTTON] = false;
	}
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void application::processInput()
{
	POINT p;
	if (GetCursorPos(&p))
	{
		//cursor position now in p.x and p.y
		if (ScreenToClient(hwnd, &p))
		{
			MousePos = p;
			//swap y
			MousePos.y = graphics->height - MousePos.y - 1;
			//p.x and p.y are now relative to hwnd's client area
		}
	}
	mainForm->onHover(cvec2i(MousePos.x, MousePos.y));
	for (vk keyCode = 0x0; keyCode < 0xff; keyCode++)
	{
		if (pressed(keyCode))
		{
			mainForm->onKeyPress(keyCode);
		}
	}
}

void application::draw()
{
	graphics->ClearColor(colorPalette::black);
	mainForm->Draw(cvec2i(0, 0), *graphics);
}

void application::MakeSurface(HWND hwnd)
{

	//adjust graphics size
	RECT graphicsrect, windowrect;
	GetClientRect(hwnd, &graphicsrect);
	GetWindowRect(hwnd, &windowrect);
	graphics = new graphicsObject();
	graphics->width = graphicsrect.right - graphicsrect.left;
	graphics->height = graphicsrect.bottom - graphicsrect.top;

	/* Use CreateDIBSection to make a HBITMAP which can be quickly
	 * blitted to a surface while giving 100% fast access to graphics->colors
	 * before blit.
	 */
	 // Desired bitmap properties
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);//sizeof(BITMAPINFO);
	bmi.bmiHeader.biWidth = graphics->width;
	bmi.bmiHeader.biHeight = graphics->height; //-graphics->height to order graphics->colors from top to bottom
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32; // last byte not used, 32 bit for alignment
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = 0;// graphics->width* graphics->height * 4;
	bmi.bmiHeader.biXPelsPerMeter = 0;
	bmi.bmiHeader.biYPelsPerMeter = 0;
	bmi.bmiHeader.biClrUsed = 0;
	bmi.bmiHeader.biClrImportant = 0;
	bmi.bmiColors[0].rgbBlue = 0;
	bmi.bmiColors[0].rgbGreen = 0;
	bmi.bmiColors[0].rgbRed = 0;
	bmi.bmiColors[0].rgbReserved = 0;
	HDC hdc = GetDC(hwnd);
	graphics->colors = nullptr;
	// Create DIB section to always give direct access to colors
	hbmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&graphics->colors, NULL, 0);

	DeleteDC(hdc);
	// Give plenty of time for main thread to finish setting up
	Sleep(50);//time??? without sleep, it finishes
	ShowWindow(hwnd, SW_SHOW);
	// Retrieve the window's DC
	wndDC = GetDC(hwnd);
	// Create DC with shared colors to variable 'graphics->colors'
	hdcMem = CreateCompatibleDC(wndDC);//HDC must be wndDC!! :)
	hbmOld = (HBITMAP)SelectObject(hdcMem, hbmp);

	graphics->depthbuffer = (fp*)calloc(graphics->width * graphics->height, sizeof(fp));
}

application* application::getApplicationConnected(HWND mainWindow)
{
	//getwindowlong is not going to cut it. long_ptr it is!
	LONG_PTR l = GetWindowLongPtr(mainWindow, GWLP_USERDATA);
	return (application*)l;
}
