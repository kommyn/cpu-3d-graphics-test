#include "EngineWindows.h"

EngineWindows::EngineWindows() {
	m_hWnd = NULL;
	m_windowClass = NULL;
	m_mainHdc = NULL;
	m_backHdc = NULL;
	m_backBitmap = NULL;
	m_screenWidth = 0;
	m_screenHeight = 0;
	m_pixelsWNum = 0;
	m_pixelsHNum = 0;
	m_pixelWidth = 0;
	m_pixelHeight = 0;
	m_isEngineActive = false;
	m_pvBits = NULL;
	m_elapsedTime = 1;
	m_windowName = L"EngineWindowClass";
	m_time = 1;
	m_onDraw = NULL;
	m_hInstance = GetModuleHandle(NULL);
	if (!m_hInstance) HandleError(L"Cannot get module handle");
}

EngineWindows::~EngineWindows() {
	ReleaseData();
}

void EngineWindows::ReleaseData() {
	ReleaseDC(m_hWnd, m_mainHdc);
	DeleteDC(m_backHdc);
	DeleteObject(m_backBitmap);
	DestroyWindow(m_hWnd);
	UnregisterClass(m_windowName.c_str(), m_hInstance);
}

LRESULT CALLBACK EngineWindows::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static EngineWindows* mainEngine;

	switch (uMsg)
	{
	case WM_CREATE:
		mainEngine = static_cast<EngineWindows*>((reinterpret_cast<CREATESTRUCT*>(lParam))->lpCreateParams);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
	case WM_KEYUP:
		if (wParam == VK_ESCAPE) {
			mainEngine->m_isEngineActive = false;
			DestroyWindow(hWnd);
		}
		mainEngine->KeyPressHandler(mainEngine, wParam, lParam);
		return 0;
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
		MouseClickHandler(mainEngine, wParam, lParam, MouseButton::LEFT);
		return 0;
	case WM_RBUTTONUP:
	case WM_RBUTTONDOWN:
		MouseClickHandler(mainEngine, wParam, lParam, MouseButton::RIGHT);
		return 0;
	case WM_MBUTTONUP:
	case WM_MBUTTONDOWN:
		MouseClickHandler(mainEngine, wParam, lParam, MouseButton::MIDDLE);
		return 0;
	case WM_XBUTTONUP:
	case WM_XBUTTONDOWN:
		MouseClickHandler(mainEngine, wParam, lParam, MouseButton::XBUTTON);
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

#define GET_X_COORD_LPARAM(lParam) (int)(short)(lParam & 0xFFFF)
#define GET_Y_COORD_LPARAM(lParam) (int)(short)((lParam >> 16) & 0xFFFF)

void EngineWindows::MouseClickHandler(EngineWindows* engine, const WPARAM& wParam, const LPARAM& lParam, MouseButton clickedButton) {
	const int xPos = GET_X_COORD_LPARAM(lParam);
	const int yPos = GET_Y_COORD_LPARAM(lParam);

	MouseFlagsStatus status;
	status.ctrlDown = wParam & MK_CONTROL;
	status.shiftDown = wParam & MK_SHIFT;
	status.leftDown = wParam & MK_LBUTTON;
	status.rightDown = wParam & MK_RBUTTON;
	status.middleDown = wParam & MK_MBUTTON;
	status.xbutton1Down = wParam & MK_XBUTTON1;
	status.xbutton2Down = wParam & MK_XBUTTON2;
	engine->OnMouseClick(clickedButton, xPos, yPos, status);
}

void EngineWindows::MouseMoveHandler(EngineWindows* engine, WPARAM wParam, LPARAM lParam) {
	const int xPos = GET_X_COORD_LPARAM(lParam);
	const int yPos = GET_Y_COORD_LPARAM(lParam);

	MouseFlagsStatus status;
	status.ctrlDown = wParam & MK_CONTROL;
	status.shiftDown = wParam & MK_SHIFT;
	status.leftDown = wParam & MK_LBUTTON;
	status.rightDown = wParam & MK_RBUTTON;
	status.middleDown = wParam & MK_MBUTTON;
	status.xbutton1Down = wParam & MK_XBUTTON1;
	status.xbutton2Down = wParam & MK_XBUTTON2;
	engine->OnMouseMove(xPos, yPos);
	std::cout << "ass" << std::endl;
}

#include <windowsx.h>

void EngineWindows::KeyPressHandler(EngineWindows* engine, WPARAM wParam, LPARAM lParam) {
	bool prevState = (lParam >> 30) & 0x1;
	bool buttonState = !((lParam >> 31) & 0x1);
	engine->OnKeyPress(wParam, prevState, buttonState);
}

bool EngineWindows::CreateGameWindow(const int& screenWidth, const int& screenHeight, const int& pixelWidth, const int& pixelHeight) {
	// TODO: That is stupid, rewrite it to correct logic (I already know screenWidth and screenHeight, there is no need for extra calculation)
	m_pixelWidth = pixelWidth;
	m_pixelHeight = pixelHeight;
	m_pixelsWNum = screenWidth / pixelWidth;
	m_pixelsHNum = screenHeight / pixelHeight;
	m_screenWidth = m_pixelsWNum * pixelWidth;
	m_screenHeight = m_pixelsHNum * pixelHeight;

	WNDCLASSEX wndClassEx;
	wndClassEx.cbSize = sizeof(WNDCLASSEX);
	wndClassEx.style = CS_GLOBALCLASS;
	wndClassEx.lpfnWndProc = WndProc;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = m_hInstance;
	wndClassEx.hIcon = NULL;
	wndClassEx.hCursor = NULL;
	wndClassEx.hbrBackground = NULL;
	wndClassEx.lpszMenuName = NULL;
	wndClassEx.lpszClassName = m_windowName.c_str();
	wndClassEx.hIconSm = NULL;
	m_windowClass = RegisterClassEx(&wndClassEx);
	if (!m_windowClass) return HandleError(L"Window class registration error");

	m_hWnd = CreateWindowEx(
		NULL,
		L"EngineWindowClass",
		L"Test 3D graph",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		m_screenWidth,
		m_screenHeight,
		NULL,
		NULL,
		m_hInstance,
		this
	);
	if (!m_hWnd) return HandleError(L"Creating of the main window error");

	m_mainHdc = GetDC(m_hWnd);
	if (!m_mainHdc) return HandleError(L"Getting main HDC error");
	m_backHdc = CreateCompatibleDC(m_mainHdc);
	if (!m_backHdc) return HandleError(L"Creating compatible DC error");

	BITMAPINFOHEADER bitmapInfoHeader;
	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biWidth = m_screenWidth;
	bitmapInfoHeader.biHeight = m_screenHeight;
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = 32;
	bitmapInfoHeader.biCompression = BI_RGB;
	bitmapInfoHeader.biSizeImage = 4 * m_screenWidth * m_screenHeight;
	bitmapInfoHeader.biXPelsPerMeter = 0;
	bitmapInfoHeader.biYPelsPerMeter = 0;
	bitmapInfoHeader.biClrUsed = 0;
	bitmapInfoHeader.biClrImportant = 0;
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader = bitmapInfoHeader;
	m_backBitmap = CreateDIBSection(m_mainHdc, &bitmapInfo, DIB_RGB_COLORS, &m_pvBits, NULL, NULL);
	if (!m_backBitmap) return HandleError(L"Creating DIB section error");
	if (!SelectObject(m_backHdc, m_backBitmap)) return HandleError(L"DIB setting error");

	return true;
}

void EngineWindows::StartGame() {
	m_isEngineActive = true;

	ShowWindow(m_hWnd, SW_SHOW);

	HBRUSH mainBrush = CreateSolidBrush(RGB(0, 0, 0));
	RECT mainRect = { 0, 0, m_screenWidth, m_screenHeight };

	std::wstring fps;
	MSG m_msg;
	while (m_isEngineActive) {
		ComputeElapsedTime();
		std::wstring windowTitle = m_windowName + L" + " + convertToWstring(1 / m_elapsedTime);
		SetWindowText(m_hWnd, windowTitle.c_str());

		FillRect(m_backHdc, &mainRect, mainBrush);

		if (PeekMessage(&m_msg, m_hWnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);
		}

		OnMouseDetection();
		OnDraw();

		BitBlt(m_mainHdc, 0, 0, m_screenWidth, m_screenHeight, m_backHdc, 0, 0, SRCCOPY);
	}
}

Point EngineWindows::GetScreenCoordinate(const Point& coord) const {
	POINT clientCoord = { coord.x, coord.y };
	ClientToScreen(m_hWnd, &clientCoord);
	return { clientCoord.x, clientCoord.y };
}

bool EngineWindows::HandleError(const std::wstring& errorMessage) {
	ReleaseData();
	MessageBox(NULL, errorMessage.c_str(), NULL, MB_OK | MB_ICONERROR);
	return false;
}

void EngineWindows::SetPixel(const Pixel& pixel) {
	if (pixel.x < 0 || pixel.x >= m_pixelsWNum || pixel.y < 0 || pixel.y >= m_pixelsHNum) return;
	/*DWORD* bits = (DWORD*)m_pvBits;
	DWORD pixelData = bits[point.x, point.y];
	unsigned char currentRed = (pixelData & 0xFF);
	unsigned char currentGreen = (pixelData & 0xFF00) >> 8;
	unsigned char currentBlue = (pixelData & 0xFF0000) >> 16;

	float alphaFactor = point.color.a / 255.0f;
	float revAlphaFator = 1 - alphaFactor;
	unsigned int red = point.color.r * alphaFactor + currentRed * revAlphaFator;
	unsigned int green = point.color.g * alphaFactor + currentGreen * revAlphaFator;
	unsigned int blue = point.color.b * alphaFactor + currentBlue * revAlphaFator;*/

	long scaledPointX = pixel.x * m_pixelWidth;
	long scaledPointY = pixel.y * m_pixelHeight;
	for (int i = 0; i < m_pixelWidth; ++i) {
		for (int j = 0; j < m_pixelHeight; ++j) {
			((DWORD*)m_pvBits)[(scaledPointX + i) + (scaledPointY + j) * m_screenWidth] = RGBA(pixel.color.r, pixel.color.g, pixel.color.b, 255);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// All methods for callbacks initiation
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EngineWindows::OnMouseDetection() {
	if (GetActiveWindow() != m_hWnd) return;

	POINT cursorPos;
	GetCursorPos(&cursorPos);
	this->OnMouseMove(cursorPos.x, cursorPos.y);

	// This is part of the screen centering logic, that should be moved to EngineBase class
	Point screenCenter = GetScreenCoordinate({ (m_screenWidth) / 2, m_screenHeight / 2 });
	SetCursorPos(screenCenter.x, screenCenter.y);
}

