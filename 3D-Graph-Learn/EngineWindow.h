#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <iostream>
#include <chrono>
#include <sstream>
#include <thread>
#include <Windows.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

#pragma comment(lib, "Msimg32.lib")

#define RGBA(r,g,b,a)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)|(((DWORD)(BYTE)(a))<<24)))

template <typename T>
std::wstring convertToWstring(T value) {
	std::wstringstream wss;
	wss << value;
	std::wstring valueToReturn;
	wss >> valueToReturn;
	return valueToReturn;
}

const float highResClockRatio = static_cast<float>(std::chrono::high_resolution_clock::period::num) / static_cast<float>(std::chrono::high_resolution_clock::period::den);

// TODO: Create some structures to represent basic types for drawing (like point, pixel, color, triangle and another)
// TODO: Optimize drawing process by using gl/gl.h

struct RBGAColor {
	unsigned char r, g, b, a;

	RBGAColor(const unsigned char& R = 255, const unsigned char& G = 255, const unsigned char& B = 255, const unsigned char& A = 255) {
		r = R;
		g = G;
		b = B;
		a = A;
	}
};

struct Pixel {
	long x, y;
	RBGAColor color;
};

struct Texel {
	long u, v;
};

// TODO: Think about moving out viewport logic from this class to another, to make code more clear
// Part of this class that can be moved to another class:
// 1. Viewport logic: everything that concerns screen size, minimazing, maximazing window, fullscreen and window mode change
// 2. All shape drawing functions must be located in another class
// 3. Move window creating to another class that will work as the error boundary (factory pattern)
// 4. WindowProc callbacks can also can be moved to another class
// 5. Lightweight class for textures and other assets

struct ImageData {
	int width, height, channels;
	unsigned char* pixels;

	ImageData() {
		width = 0;
		height = 0;
		channels = 0;
		pixels = NULL;
	}

	~ImageData() {
		if (pixels) {
			stbi_image_free(pixels);
		}
	}
};

class EngineWindow
{
protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	ATOM m_windowClass;
	MSG m_msg;
	HDC m_mainHdc, m_backHdc;
	HBITMAP m_backBitmap;
	bool m_isEngineActive;
	int m_screenWidth;
	int m_screenHeight;
	int m_pixelsWNum;
	int m_pixelsHNum;
	int m_pixelWidth;
	int m_pixelHeight;
	void* m_pvBits;
	float m_elapsedTime;
	float m_time;
	ImageData img;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	EngineWindow();
	virtual ~EngineWindow();
	
	// TODO: Split this code into several classes according to SOLID principles, make this class abstract with update and draw methods to client
	//       for override (update will serve as callback-setting method and draw will be drawing method)
	// TODO: Maybe it's a good idea to move drawing methods to another class, or just add some wrapper method for drawing anything but SetPixel
	//       SetPixel is a basic method
	// TODO: Fix deleting of all the created data and resourses of the class if something goes wrong
	// TODO: Move all drawing logic in the another thread
	bool CreateGameWindow(const int& screenWidth, const int& screenHeight, const int& pixelWidth, const int& pixelHeight);
	// TODO: I don't like how main game cycle working right now. Also I need to do something with elapsed time calculation,
	//       I feel like there is something wrong with it for now
	// TODO: Move game cycle to another method and maybe it's a good idea to move all render process in another thread (think about it)
	void StartGame();
	bool HandleError(std::wstring errorMessage);
	void ComputeElapsedTime();

	// This method os the core of the engine, it draws one pixel considering screen ratio and draws it with it alpha channel
	void SetPixel(const Pixel& point);

	// TODO: Finally read something about Bresenham algorythm
	// TODO: Give opportunity to use interpolation of colors or to use some homogenous color (passed as the parameter)
	void DrawLine(Pixel a, Pixel b);
	// TODO: Read something about barycentric coordinates
	void DrawTriangle(Pixel a, Pixel b, Pixel c);
	void FillTriangle(Pixel a, Pixel b, Pixel c);
	void DrawSprite(Pixel a, Pixel b, std::string pathToSprite);
	// TODO: Fix texturing to enter texture values correctly in the range [0, 1]
	// TODO: Try to do something with linear and bilinear interpolation of the textures
	void TexturedTriangle(Pixel a, Pixel b, Pixel c, std::string pathToTexture, Pixel ta, Pixel tb, Pixel tc);

	// TODO: Create methods for drawing circle

	virtual void OnUpdate(const float& elapsedTime, const float& time);
};

/*
		// Star the thread
		thread t = thread(&olcConsoleGameEngine::GameThread, this);

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Wait for thread to be exited
		t.join();*/