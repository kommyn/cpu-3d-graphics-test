#pragma once

// This file should be base class for every engine class

#include <chrono>

// This const value is used for elapsed time calculation
const double highResClockRatio = static_cast<double>(std::chrono::high_resolution_clock::period::num) / std::chrono::high_resolution_clock::period::den;

// This structure represents point on the screen
struct Point {
	int x, y;
};

// This structure represents RGBA color
struct RGBAColor {
	unsigned char r, g, b, a;
};

// This structure represents pixel color and position on screen
struct Pixel {
	int x, y;
	RGBAColor color;
};

// TODO: Change it to something multiplatform
struct MouseFlagsStatus {
	bool ctrlDown;
	bool shiftDown;
	bool leftDown;
	bool rightDown;
	bool middleDown;
	bool xbutton1Down;
	bool xbutton2Down;
};

// TODO: Change it to something multiplatform
enum class MouseButton {
	LEFT,
	RIGHT,
	MIDDLE,
	XBUTTON
};

class EngineBase
{
protected:
	double m_time;
	float m_elapsedTime;
public:
	EngineBase();
	virtual ~EngineBase() = default;

	virtual bool CreateGameWindow(const int& screenWidth, const int& screenHeight, const int& pixelWidth, const int& pixelHeight) = 0;
	virtual void StartGame() = 0;

	virtual void ComputeElapsedTime();
	virtual void SetPixel(const Pixel& point) = 0;

	virtual void OnMouseClick(const MouseButton& button, const int& xPos, const int& yPos, const MouseFlagsStatus& status) = 0;
	virtual void OnMouseMove(const int& xPos, const int& yPos) = 0;
	virtual void OnKeyPress(const unsigned int& wParam, const bool& prevState, const bool& buttonState) = 0;
	virtual void OnResize() = 0;
	virtual void OnDraw() = 0;

	virtual Point GetScreenCoordinate(const Point& coord) const = 0;
};

