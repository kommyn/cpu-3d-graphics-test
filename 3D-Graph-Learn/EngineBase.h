#pragma once

// This file should be base class for every engine class

#include <chrono>

// This const value is used for elapsed time calculation
const float highResClockRatio = static_cast<float>(std::chrono::high_resolution_clock::period::num) / std::chrono::high_resolution_clock::period::den;

// This structure represents RGBA color
struct RGBAColor {
	unsigned char r, g, b, a;
};

// This structure represents pixel color and position on screen
struct Pixel {
	int x, y;
	RGBAColor color;
};

class EngineBase
{
protected:
	float m_time;
	float m_elapsedTime;
public:
	EngineBase();
	virtual ~EngineBase() = default;

	virtual bool CreateGameWindow(const int& screenWidth, const int& screenHeight, const int& pixelWidth, const int& pixelHeight) = 0;
	virtual void StartGame() = 0;

	virtual void ComputeElapsedTime();
	virtual void SetPixel(const Pixel& point) = 0;

	virtual void OnMouseClick() = 0;
	virtual void OnKeyPress() = 0;
	virtual void OnResize() = 0;
	virtual void OnDraw() = 0;
};

