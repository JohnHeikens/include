#include "GlobalFunctions.h"

void makelowercase(std::wstring& s) 
{
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); });
}
color operator*(color c, cfp multiplier)
{
	return color((byte)(c.r * multiplier), (byte)(c.g * multiplier), (byte)(c.b * multiplier));
}
//https://stackoverflow.com/questions/4654636/how-to-determine-if-a-wstring-is-a-number-with-c
bool strtol(const std::wstring line, long& number, int radix)
{
	wchar_t* p;
	number = wcstol(line.c_str(), &p, radix);
	return *p == 0;
}
//base 10
bool strtod(const std::wstring line, fp& number)
{
	wchar_t* p;
	number = wcstod(line.c_str(), &p);
	return *p == 0;
}
bool operator==(const color& a, const color& b)
{
	return a.val == b.val;
}
int Math_Fix(fp f) {
	int i = (int)f;
	return i < f ? i++ : i;
}

int FloorToBase(int val, int base)
{
	int floored = 1;
	while (floored * base < val)
	{
		floored *= base;
	}
	return floored;
}

// Get the horizontal and vertical screen sizes in pixel
void GetMonitorResolution(int& horizontal, int& vertical)
{
	RECT desktop = GetMonitorRect();
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}
//source:
//http://www.cplusplus.com/forum/windows/190177/
RECT GetDesktopRect()
{
	RECT desktop;
	// size of screen (primary monitor) without just the taskbar
	BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA, 0, &desktop, 0);
	return desktop;
}
RECT GetMonitorRect()
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	return desktop;
}
//resizes the array to a new array and deletes the old array
byte* ResizeArray(byte* ptr, cint newsize)
{
	byte* newArray = new byte[newsize];
	memcpy(newArray, ptr, newsize);
	//CopyArray(ptr, ptr + newsize, newArray);
	delete[] ptr;
	return newArray;
}
//source:
//https://stackoverflow.com/questions/10873382/write-and-read-string-to-binary-file-c
void writeStringToStream(std::ofstream& const stream , const std::string str) {
	int size = str.size();
	stream.write(castout(&size), sizeof(size));
	stream.write(&str[0], size);
}

std::string readStringFromStream(std::ifstream& stream) {
	std::string str((std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>());
	/*std::string str;
	int size;
	stream.read(castin(&size), sizeof(size));
	str.resize(size);
	stream.read(&str[0], size);*/
	return str;
}
void writeWStringToStream(std::ofstream& const stream, const std::wstring str) {
	writeStringToStream(stream, WStringToString(str));
}

std::wstring readWStringFromStream(std::ifstream& stream) {
	return StringToWString(readStringFromStream(stream));
}
//source:
//https://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file
std::vector<char> readBMP(const std::wstring& file, int& width, int& height, int& channels)
{
	static constexpr int HEADER_SIZE = 54;

	std::ifstream stream(file, std::ios::binary);

	std::array<char, HEADER_SIZE> header;
	stream.read(header.data(), header.size());

	auto fileSize = *reinterpret_cast<uint32_t*>(&header[2]);
	auto dataOffset = *reinterpret_cast<uint32_t*>(&header[10]);
	width = *reinterpret_cast<uint32_t*>(&header[18]);
	height = *reinterpret_cast<uint32_t*>(&header[22]);
	auto bpp = *reinterpret_cast<uint16_t*>(&header[28]);
	channels = (int)ceil(bpp / 8);

	std::vector<char> img(dataOffset - HEADER_SIZE);
	stream.read(img.data(), img.size());
	//32 or 24
	auto dataSize = bpp == 32 ? width * height * channels : ((width * 3 + 3) & (~3)) * height;

	img.resize(dataSize);
	stream.read(img.data(), img.size());

	//switch red and blue
	/*char temp = 0;

	for (auto i = dataSize - 4; i >= 0; i -= 3)
	{
		temp = img[i];
		img[i] = img[i + 2];
		img[i + 2] = temp;

		//std::cout << "R: " << int(img[i] & 0xff) << " G: " << int(img[i + 1] & 0xff) << " B: " << int(img[i + 2] & 0xff) << std::endl;
	}*/

	return img;
}
void InsertChannel(byte* arrayPtr, byte* ArrayEndPtr, byte* newarrayPtr, int start, int stride, byte value)
{
	int i = 0;
	while (arrayPtr < ArrayEndPtr)
	{
		if ((i++) % stride == start)
		{
			*newarrayPtr++ = value;
		}
		else
		{
			*newarrayPtr++ = *arrayPtr++;
		}
	}
	if (i % stride == start) 
	{
		*newarrayPtr++ = value;
	}
}
uint* getplaneindices(cint planecount)
{
	uint* indices = new uint[planecount * 6];
	uint* indptr = indices;
	int vertcount = planecount * 4;
	for (int i = 0; i < vertcount; i += 4)
	{
		*indptr++ = i;
		*indptr++ = i + 1;
		*indptr++ = i + 2;
		*indptr++ = i + 3;
		*indptr++ = i + 2;
		*indptr++ = i + 1;
	}
	return indices;
}
//get the graphics rect with getclientrect(HWND, &drawrect)
HWND MakeWindow(RECT windowrect,
	HINSTANCE hInstance,
	WNDPROC proc, void* userData, bool fullScreen
)
{
	WNDCLASSEX wc;
	//MSG msg;
	// Init wc
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = CreateSolidBrush(0);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = proc;
	wc.lpszClassName = L"animation_class";
	wc.lpszMenuName = NULL;
	wc.style = 0;
	// Register wc
	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Failed to register window class.", L"Error", MB_OK);
		return 0;
	}
	// Make window:
	//https://docs.microsoft.com/en-us/windows/win32/learnwin32/creating-a-window
	HWND hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		L"animation_class",
		L"Animation",
		fullScreen ? 
		WS_POPUP : 
		WS_MINIMIZEBOX | WS_SYSMENU | WS_POPUP | WS_CAPTION,
		0, 0, windowrect.right - windowrect.left, windowrect.bottom - windowrect.top,
		NULL, NULL, hInstance,
		userData);//additional application data

	//POINT ptDiff;
	//RECT drawarea;
	// Get window and client sizes
	//GetClientRect(hwnd, &drawarea);
	//GetWindowRect(hwnd, &windowrect);
	// Find offset between window size and client size
	//ptDiff.x = (windowrect.right - windowrect.left) - drawarea.right;
	//ptDiff.y = (windowrect.bottom - windowrect.top) - drawarea.bottom;
	// Resize client
	//MoveWindow(hwnd, windowrect.left, windowrect.top, (windowrect.right - windowrect.left) + ptDiff.x, (windowrect.bottom - windowrect.top) + ptDiff.y, false);
	//UpdateWindow(hwnd);
	return hwnd;
}