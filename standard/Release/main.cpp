#include "main.h"
#include <time.h>
#include <list>

GraphicsObject* graphics = new GraphicsObject();

// Global Windows/Drawing variables
HBITMAP hbmp = NULL;
HWND hwnd = NULL;
HDC hdcMem = NULL;
// The window's DC
HDC wndDC = NULL;
HBITMAP hbmOld = NULL;

POINT MousePos = POINT();

int Run() 
{
	srand(GetSecond());
	while (DoEvents())//next frame
	{
		ProcessInput();//process events from user
		// Do stuff with graphics->colors
		Draw();
		// Draw graphics->colors to window
		BitBlt(wndDC, 0, 0, graphics->width, graphics->height, hdcMem, 0, 0, SRCCOPY);
	}
	return 0;
}

void Draw() 
{

}

void ProcessInput()
{
	POINT p;
	if (GetCursorPos(&p))
	{
		//cursor position now in p.x and p.y
		if (ScreenToClient(hwnd, &p))
		{
			MousePos = p;
			//p.x and p.y are now relative to hwnd's client area
		}
	}
}
void MakeSurface(HWND hwnd) {

	//adjust graphics size
	RECT graphicsrect, windowrect;
	GetClientRect(hwnd, &graphicsrect);
	GetWindowRect(hwnd, &windowrect);
	graphics = DBG_NEW GraphicsObject();
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
	bmi.bmiHeader.biHeight = -graphics->height; // Order graphics->colors from top to bottom
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
	hbmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)& graphics->colors, NULL, 0);
	
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
	//set random seed
	srand(time(NULL));
}
LRESULT CALLBACK WndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (msg) {
	case WM_CREATE:
	{
		MakeSurface(hwnd);
	}
	break;
	case WM_MOUSEMOVE:
	{
		
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		// Draw graphics->colors to window when window needs repainting
		BitBlt(hdc, 0, 0, graphics->width, graphics->height, hdcMem, 0, 0, SRCCOPY);
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
		SelectObject(hdcMem, hbmOld);
		DeleteDC(wndDC);
		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	//get global vars
	hwnd = MakeWindow(GetMonitorRect(), hInstance, WndProc);
	//graphics->width = 1280;
	//graphics->height = 640;
	
	// Resize client
	return Run();
}