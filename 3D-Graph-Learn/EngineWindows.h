#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <string>
#include <sstream>
#include <functional>
#include <iostream>
#include <Windows.h>

#include "EngineBase.h"

#pragma comment(lib, "Msimg32.lib")

#define RGBA(r,g,b,a)          ((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)|(((DWORD)(BYTE)(a))<<24)))

// TODO: Move it to utlis file
template <typename T>
std::wstring convertToWstring(T value) {
	std::wstringstream wss;
	wss << value;
	std::wstring valueToReturn;
	wss >> valueToReturn;
	return valueToReturn;
}


class EngineWindows : public EngineBase
{
private:
	// TODO: Workaround click and press messages handling (I don't like how it's currently working, maybe it's a good idea to look at SFML and glfw)
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void MouseClickHandler(EngineWindows* engine, const WPARAM& wParam, const LPARAM& lParam, MouseButton clickedButton);
	static void MouseMoveHandler(EngineWindows* engine, WPARAM wParam, LPARAM lParam);
	static void KeyPressHandler(EngineWindows* engine, WPARAM wParam, LPARAM lParam);

	void ReleaseData();
protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	ATOM m_windowClass;
	HDC m_mainHdc, m_backHdc;
	HBITMAP m_backBitmap;
	std::wstring m_windowName;
	bool m_isEngineActive;
	int m_screenWidth;
	int m_screenHeight;
	int m_pixelsWNum;
	int m_pixelsHNum;
	int m_pixelWidth;
	int m_pixelHeight;
	void* m_pvBits;
	std::function<void()> m_onDraw;

	// TODO: Refactor window creation errors handling with exceptions mechanism
	bool HandleError(const std::wstring& errorMessage);
public:
	EngineWindows();
	virtual ~EngineWindows() override;

	virtual bool CreateGameWindow(const int& screenWidth, const int& screenHeight, const int& pixelWidth, const int& pixelHeight) override;
	virtual void StartGame() override;

	virtual void SetPixel(const Pixel& pixel) override;

	void OnDraw() override {};
	void OnMouseMove(const int& xPos, const int& yPos) override {};
	void OnMouseClick(const MouseButton& button, const int& xPos, const int& yPos, const MouseFlagsStatus& status) override {};
	void OnKeyPress(const unsigned int& wParam, const bool& prevState, const bool& buttonState) override {};
	void OnResize() override {};
	// TODO: Move screen centering logic to virtual method in EngineBase class
	void OnMouseDetection();

	Point GetScreenCoordinate(const Point& coord) const override;
};
