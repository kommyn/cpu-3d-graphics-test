#include "EngineWindow.h"

EngineWindow::EngineWindow() {
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
	m_time = 1;
	m_hInstance = GetModuleHandle(NULL);
}

EngineWindow::~EngineWindow() {
	ReleaseDC(m_hWnd, m_mainHdc);
	DeleteDC(m_backHdc);
	DeleteObject(m_backBitmap);
}

bool EngineWindow::CreateGameWindow(const int& screenWidth, const int& screenHeight, const int& pixelWidth, const int& pixelHeight) {
	m_pixelWidth = pixelWidth;
	m_pixelHeight = pixelHeight;
	m_pixelsWNum = screenWidth / pixelWidth;
	m_pixelsHNum = screenHeight / pixelHeight;
	m_screenWidth = m_pixelsWNum * pixelWidth;
	m_screenHeight = m_pixelsHNum * pixelHeight;

	WNDCLASSEX wndClassEx;
	wndClassEx.cbSize = sizeof(WNDCLASSEX);
	wndClassEx.style = CS_GLOBALCLASS;
	wndClassEx.lpfnWndProc = WindowProc;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = m_hInstance;
	wndClassEx.hIcon = NULL;
	wndClassEx.hCursor = NULL;
	wndClassEx.hbrBackground = NULL;
	wndClassEx.lpszMenuName = NULL;
	wndClassEx.lpszClassName = L"EngineWindowClass";
	wndClassEx.hIconSm = NULL;
	m_windowClass = RegisterClassEx(&wndClassEx);
	if (!m_windowClass) return HandleError(L"Window class registration error");

	m_hWnd = CreateWindowEx(
		NULL,
		L"EngineWindowClass",
		L"Test 3D graph",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
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

void EngineWindow::StartGame() {
	m_isEngineActive = true;

	ShowWindow(m_hWnd, SW_SHOW);

	HBRUSH mainBrush = CreateSolidBrush(RGB(0, 0, 0));
	RECT mainRect = { 0, 0, m_screenWidth, m_screenHeight };

	int i = 0;

	std::wstring fps;
	MSG m_msg;
	while (m_isEngineActive) {
		ComputeElapsedTime();
		fps = convertToWstring(1 / m_elapsedTime);
		SetWindowText(m_hWnd, fps.c_str());

		FillRect(m_backHdc, &mainRect, mainBrush);

		if (PeekMessage(&m_msg, m_hWnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);
		}

		//if (i == 0) {
		OnUpdate(m_elapsedTime, m_time);
		//}
		//++i;
		BitBlt(m_mainHdc, 0, 0, m_screenWidth, m_screenHeight, m_backHdc, 0, 0, SRCCOPY);
	}
}

bool EngineWindow::HandleError(std::wstring errorMessage) {
	MessageBox(NULL, errorMessage.c_str(), NULL, MB_OK | MB_ICONERROR);
	return false;
}

void EngineWindow::ComputeElapsedTime() {
	float time = highResClockRatio * std::chrono::high_resolution_clock::now().time_since_epoch().count();
	m_elapsedTime = time - m_time;
	m_time = time;
}

void EngineWindow::SetPixel(const Pixel& point) {
	if (point.x < 0 || point.x >= m_pixelsWNum || point.y < 0 || point.y >= m_pixelsHNum) return;
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

	long scaledPointX = point.x * m_pixelWidth;
	long scaledPointY = point.y * m_pixelHeight;
	for (int i = 0; i < m_pixelWidth; ++i) {
		for (int j = 0; j < m_pixelHeight; ++j) {
			((DWORD*)m_pvBits)[(scaledPointX + i) + (scaledPointY + j) * m_screenWidth] = RGBA(point.color.r, point.color.g, point.color.b, 255);
		}
	}
}

void EngineWindow::DrawLine(Pixel a, Pixel b) {
	bool steep = std::abs(b.y - a.y) > std::abs(b.x - a.x);
	if (steep) {
		std::swap(a.x, a.y);
		std::swap(b.x, b.y);
	}
	if (a.x > b.x) {
		std::swap(a.x, b.x);
		std::swap(a.y, b.y);
		std::swap(a.color.r, b.color.r);
		std::swap(a.color.g, b.color.g);
		std::swap(a.color.b, b.color.b);
		std::swap(a.color.a, b.color.a);
	}
	int dx = b.x - a.x;
	int dy = std::abs(b.y - a.y);
	int error = dx / 2;
	int yStep = (a.y < b.y) ? 1 : -1;
	int y = a.y;

	for (int x = a.x; x <= b.x; ++x) {
		int pointX = steep ? y : x;
		int pointY = steep ? x : y;
		float colX = (pointX - (steep ? a.y : a.x)) / (steep ? (float)dy : (float)dx);
		float colY = (pointY - (steep ? a.x : a.y)) / (steep ? (float)dx : (float)dy);
		float percent = std::sqrt((colX * colX + colY * colY) / 2);
		//SetPixel({ pointX, pointY, (unsigned char)(b.r * percent + a.r * (1 - percent)), (unsigned char)(b.g * percent + a.g * (1 - percent)), (unsigned char)(b.b * percent + b.b * (1 - percent)) });
		SetPixel({ pointX, pointY, { 200, 200, 200, 255 } });
		error -= dy;
		if (error < 0) {
			y += yStep;
			error += dx;
		}
	}
}

void EngineWindow::DrawTriangle(Pixel a, Pixel b, Pixel c) {
	DrawLine(a, b);
	DrawLine(b, c);
	DrawLine(c, a);
}

void EngineWindow::FillTriangle(Pixel a, Pixel b, Pixel c) {
	if (a.y > b.y) {
		std::swap(a, b);
	}
	if (a.y > c.y) {
		std::swap(a, c);
	}
	if (b.y > c.y) {
		std::swap(b, c);
	}

	// Calculate color interpolation for three points
	// TODO: Clean code in here
	float delta = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
	float redA = ((b.color.r - a.color.r) * (c.y - a.y) - (c.color.r - a.color.r) * (b.y - a.y)) / delta;
	float redB = ((b.x - a.x) * (c.color.r - a.color.r) - (c.x - a.x) * (b.color.r - a.color.r)) / delta;
	float redC = (a.x * (b.y * c.color.r - c.y * b.color.r) - a.y * (b.x * c.color.r - c.x * b.color.r) + a.color.r * (b.x * c.y - c.x * b.y)) / delta;
	float greenA = ((b.color.g - a.color.g) * (c.y - a.y) - (c.color.g - a.color.g) * (b.y - a.y)) / delta;
	float greenB = ((b.x - a.x) * (c.color.g - a.color.g) - (c.x - a.x) * (b.color.g - a.color.g)) / delta;
	float greenC = (a.x * (b.y * c.color.g - c.y * b.color.g) - a.y * (b.x * c.color.g - c.x * b.color.g) + a.color.g * (b.x * c.y - c.x * b.y)) / delta;
	float blueA = ((b.color.b - a.color.b) * (c.y - a.y) - (c.color.b - a.color.b) * (b.y - a.y)) / delta;
	float blueB = ((b.x - a.x) * (c.color.b - a.color.b) - (c.x - a.x) * (b.color.b - a.color.b)) / delta;
	float blueC = (a.x * (b.y * c.color.b - c.y * b.color.b) - a.y * (b.x * c.color.b - c.x * b.color.b) + a.color.b * (b.x * c.y - c.x * b.y)) / delta;

	// Coeffitients for the longest part of the triangle
	float longestA = (float)(c.x - a.x) / (c.y - a.y);
	float longestB = a.x - longestA * a.y;

	long secondSegmentHeight = b.y - a.y;
	if (secondSegmentHeight != 0) {
		// Coeffitients for the first line part of the triangle
		float secondSegmentA = (float)(b.x - a.x) / secondSegmentHeight;
		float secondSegmentB = a.x - secondSegmentA * a.y;

		for (long y = a.y; y <= b.y; ++y) {
			long xMin = secondSegmentA * y + secondSegmentB + 0.5;
			long xMax = longestA * y + longestB + 0.5;
			if (xMin > xMax) std::swap(xMin, xMax);
			for (long x = xMin; x <= xMax; ++x) {
				float fRed = redA * x + redB * y + redC;
				float fGreen = greenA * x + greenB * y + greenC;
				float fBlue = blueA * x + blueB * y + blueC;
				unsigned char red = 0;
				unsigned char green = 0;
				unsigned char blue = 0;
				if (fRed > 255) {
					red = 255;
				}
				else if (fRed < 0) {
					red = 0;
				}
				else red = fRed;
				if (fGreen > 255) {
					green = 255;
				}
				else if (fGreen < 0) {
					green = 0;
				}
				else green = fGreen;
				if (fBlue > 255) {
					blue = 255;
				}
				else if (fBlue < 0) {
					blue = 0;
				}
				else blue = fBlue;
				SetPixel({ x, y, { red, green, blue, 255 } });
			}
		}
	}

	long firstSegmentHeight = c.y - b.y;
	if (firstSegmentHeight != 0) {
		// Coeffitients for the second line part of the triangle
		float firstSegmentA = (float)(c.x - b.x) / firstSegmentHeight;
		float firstSegmentB = b.x - firstSegmentA * b.y;

		for (long y = b.y; y <= c.y; ++y) {
			long xMin = firstSegmentA * y + firstSegmentB + 0.5;
			long xMax = longestA * y + longestB + 0.5;
			if (xMin > xMax) std::swap(xMin, xMax);
			for (long x = xMin; x <= xMax; ++x) {
				float fRed = redA * x + redB * y + redC;
				float fGreen = greenA * x + greenB * y + greenC;
				float fBlue = blueA * x + blueB * y + blueC;
				unsigned char red = 0;
				unsigned char green = 0;
				unsigned char blue = 0;
				if (fRed > 255) {
					red = 255;
				}
				else if (fRed < 0) {
					red = 0;
				}
				else red = fRed;
				if (fGreen > 255) {
					green = 255;
				}
				else if (fGreen < 0) {
					green = 0;
				}
				else green = fGreen;
				if (fBlue > 255) {
					blue = 255;
				}
				else if (fBlue < 0) {
					blue = 0;
				}
				else blue = fBlue;
				SetPixel({ x, y, { red, green, blue, 255 } });
			}
		}
	}
}

void EngineWindow::DrawSprite(Pixel a, Pixel b, std::string pathToSprite) {
	unsigned char* pixels;
	if (!img.pixels) img.pixels = stbi_load(pathToSprite.c_str(), &img.width, &img.height, &img.channels, 4);

	if (!img.pixels) {
		std::cout << "IMAGE_ERROR::LOADING: " << pathToSprite << std::endl;
		return;
	}

	//float kx = (float)img.width / (b.x - a.x);
	//float bx = (float)(a.x * img.width) / (a.x - b.x);
	//float ky = (float)img.height / (b.y - a.y);
	//float by = (float)(a.y * img.height) / (a.y - b.y);
	float xFactor = img.width / (float)m_pixelsWNum;
	float yFactor = img.height / (float)m_pixelsHNum;

	for (int x = a.x; x < b.x; ++x) {
		long textureX = x * xFactor;
		for (int y = a.y; y < b.y; ++y) {
			long textureY = y * yFactor;
			long textureStartIndex = (textureX + textureY * img.width) * 4;
			unsigned char red = img.pixels[textureStartIndex + 2];
			unsigned char green = img.pixels[textureStartIndex + 1];
			unsigned char blue = img.pixels[textureStartIndex];
			unsigned char alpha = img.pixels[textureStartIndex + 3];
			SetPixel({ x, b.y - y - 1, { red, green, blue, alpha } });
		}
	}
}

void EngineWindow::OnUpdate(const float& elapsedTime, const float& time) {
	/*long point1x = 40 * std::abs(std::sin(time));

	Pixel point1 = { 40, 40, { 255, 0, 0, 255 } };
	Pixel point2 = { point1x + 40, 320, { 0, 255, 0, 255 } };
	Pixel point3 = { 280, 160, { 0, 0, 255, 255 } };
	Pixel point4 = { 360, 120, { 100, 100, 100, 255 } };

	FillTriangle(point1, point2, point3);
	FillTriangle(point3, point4, point1);
	FillTriangle(point2, point3, point4);*/
	//2048 * 1536
	TexturedTriangle({ 0, 0 }, { 0, m_screenHeight }, { m_screenWidth, m_screenHeight }, "war.jpg", { 0, 0 }, { 0, 1535 }, { 2047, 1535 });
	TexturedTriangle({ 0, 0 }, { m_screenWidth, m_screenHeight }, { m_screenWidth, 0 }, "war.jpg", { 0, 0 }, { 2047, 1535 }, { 2047, 0 });

	//DrawTriangle(point1, point2, point3);
	//DrawTriangle(point3, point4, point1);
	//DrawTriangle(point2, point3, point4);
}

LRESULT CALLBACK EngineWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static EngineWindow* mainEngine;

	switch (uMsg)
	{
	case WM_CREATE:
		mainEngine = static_cast<EngineWindow*>((reinterpret_cast<CREATESTRUCT*>(lParam))->lpCreateParams);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			mainEngine->m_isEngineActive = false;
			DestroyWindow(hwnd);
		}
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void EngineWindow::TexturedTriangle(Pixel a, Pixel b, Pixel c, std::string pathToTexture, Pixel ta, Pixel tb, Pixel tc) {
	if (!img.pixels) {
		img.pixels = stbi_load(pathToTexture.c_str(), &img.width, &img.height, &img.channels, 4);
	}

	if (!img.pixels) {
		std::cout << "ERROR::IMAGE_LOADING: " << pathToTexture << std::endl;
		return;
	}

	if (a.y > b.y) {
		std::swap(a, b);
		std::swap(ta, tb);
	}
	if (a.y > c.y) {
		std::swap(a, c);
		std::swap(ta, tc);
	}
	if (b.y > c.y) {
		std::swap(b, c);
		std::swap(tb, tc);
	}

	float delta = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
	float txA = ((tb.x - ta.x) * (c.y - a.y) - (tc.x - ta.x) * (b.y - a.y)) / delta;
	float txB = ((b.x - a.x) * (tc.x - ta.x) - (c.x - a.x) * (tb.x - ta.x)) / delta;
	float txC = (a.x * (b.y * tc.x - c.y * tb.x) - a.y * (b.x * tc.x - c.x * tb.x) + ta.x * (b.x * c.y - c.x * b.y)) / delta;
	float tyA = ((tb.y - ta.y) * (c.y - a.y) - (tc.y - ta.y) * (b.y - a.y)) / delta;
	float tyB = ((b.x - a.x) * (tc.y - ta.y) - (c.x - a.x) * (tb.y - ta.y)) / delta;
	float tyC = (a.x * (b.y * tc.y - c.y * tb.y) - a.y * (b.x * tc.y - c.x * tb.y) + ta.y * (b.x * c.y - c.x * b.y)) / delta;

	// Coeffitients for the longest part of the triangle
	float longestA = (float)(c.x - a.x) / (c.y - a.y);
	float longestB = a.x - longestA * a.y;

	long secondSegmentHeight = b.y - a.y;
	if (secondSegmentHeight > 0) {
		// Coeffitients for the first line part of the triangle
		float secondSegmentA = (float)(b.x - a.x) / secondSegmentHeight;
		float secondSegmentB = a.x - secondSegmentA * a.y;

		for (long y = a.y; y <= b.y; ++y) {
			long xMin = secondSegmentA * y + secondSegmentB + 0.5;
			long xMax = longestA * y + longestB + 0.5;
			if (xMin > xMax) std::swap(xMin, xMax);
			for (long x = xMin; x <= xMax; ++x) {
				long textureX = x * txA + y * txB + txC;
				long textureY = x * tyA + y * tyB + tyC;
				long textureIndex = (textureX + textureY * img.width) * 4;
				unsigned char red = img.pixels[textureIndex + 2];
				unsigned char green = img.pixels[textureIndex + 1];
				unsigned char blue = img.pixels[textureIndex];
				unsigned char alpha = img.pixels[textureIndex + 3];
				SetPixel({ x, y, { red, green, blue, 255 } });
			}
		}
	}

	// Coeffitients for the second line part of the triangle
	long firstSegmentHeight = c.y - b.y;
	if (firstSegmentHeight > 0) {
		float firstSegmentA = (float)(c.x - b.x) / firstSegmentHeight;
		float firstSegmentB = b.x - firstSegmentA * b.y;

		for (long y = b.y; y <= c.y; ++y) {
			long xMin = firstSegmentA * y + firstSegmentB + 0.5;
			long xMax = longestA * y + longestB + 0.5;
			if (xMin > xMax) std::swap(xMin, xMax);
			for (long x = xMin; x <= xMax; ++x) {
				long textureX = x * txA + y * txB + txC;;
				long textureY = x * tyA + y * tyB + tyC;
				long textureIndex = (textureX + textureY * img.width) * 4;
				unsigned char red = img.pixels[textureIndex + 2];
				unsigned char green = img.pixels[textureIndex + 1];
				unsigned char blue = img.pixels[textureIndex];
				unsigned char alpha = img.pixels[textureIndex + 3];
				SetPixel({ x, y, { red, green, blue, 255 } });
			}
		}
	}
}