#pragma once

#include <cmath>

#include "EngineWindows.h"
#include "TexturesFactory.h"
#include "Matrix.h"
#include "GraphUtils.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

struct Polygon3Vert {
	e3Dg::Vector3f coords[3];
	e3Dg::Vector2f textCoords[3];
};

e3Dg::Matrix4x4 lookAtCalc(const e3Dg::Vector3f& from, const e3Dg::Vector3f& to, const e3Dg::Vector3f& tmp = e3Dg::Vector3f({ 0.0f, 1.0f, 0.0f }));

template <unsigned char N>
struct Polygon3v {
	union {
		e3Dg::Vector<N> vertices[3];
		struct {
			e3Dg::Vector<N> a, b, c;
		};
	};
};

class Engine3D : public EngineWindows
{
private:
	void InterpolatedHalfTriangle(const Pixel& a, const Pixel& b, float* coeffs);
	void TexturedHalfTriangle(const Pixel& a, const Pixel& b);
protected:
	TexturesFactory m_texturesFact;
	unsigned int m_buttonsStates[255];
	// TODO: Move camera logic to another class, this class should only has functionality of 3D pipeline usage
	e3Dg::Vector3f m_cameraPos;
	e3Dg::Vector3f m_cameraForward;
	e3Dg::Vector3f m_cameraUp;
	e3Dg::Vector3f m_cameraRight;
	float m_lastMouseX;
	float m_lastMouseY;
	bool m_firstRender;
	float m_cameraPitch;
	float m_cameraYaw;
	float m_cameraSpeed;
	bool m_cursorResetting;
public:
	Engine3D();
	~Engine3D() = default;

	void DrawLine(Pixel a, Pixel b);
	void DrawTriangle(const Pixel& a, const Pixel& b, const Pixel& c);
	void InterpolatedTriangle(Pixel a, Pixel b, Pixel c);
	void TexturedTriangle(Pixel a, Pixel b, Pixel c, std::string filePath);
	void VertexPipe();

	void OnMouseMove(const int& xPos, const int& yPos, const MouseFlagsStatus& status) override {
		// TODO: Add getting of the window size and border size to the EngineBase class
		//       It is just test code, ti should be moved to another class
		if (m_cursorResetting) {
			m_cursorResetting = false;
			return;
		}
		POINT point = { xPos, yPos };
		std::cout << "Before: " << std::endl;
		std::cout << "Point x: " << point.x << "; point y: " << point.y << std::endl;
		ClientToScreen(m_hWnd, &point);
		std::cout << "After: " << std::endl;
		std::cout << "Point x: " << point.x << "; point y: " << point.y << std::endl;
		RECT windowRect, clientRect;
		GetWindowRect(m_hWnd, &windowRect);
		GetClientRect(m_hWnd, &clientRect);
		const int borderThickness = ((windowRect.right - windowRect.left) - clientRect.right) / 2;
		const int borderThicknessTop = (windowRect.bottom - windowRect.top) - clientRect.bottom - borderThickness;
		const int screenCenterX = (windowRect.right + windowRect.left) / 2;
		const int screenCenterY = (windowRect.bottom + windowRect.top) / 2;
		const int diffX = xPos + windowRect.left + borderThickness - screenCenterX;
		const int diffY = yPos + windowRect.top + borderThicknessTop - screenCenterY;
		//float diffX = xPos - m_lastMouseX;
		//float diffY = yPos - m_lastMouseY;
		std::cout << "diffX2: " << diffX << std::endl;
		std::cout << "diffY2: " << diffY << std::endl;
		if (!(diffX == 0 && diffY == 0)) {
			if (m_firstRender) {
				m_firstRender = false;
				m_lastMouseX = xPos;
				m_lastMouseY = yPos;
			}
			else {
				m_lastMouseX = xPos;
				m_lastMouseY = yPos;
				const float divededPi = M_PI / 2;
				m_cameraYaw -= diffX * 0.00025;
				if (m_cameraYaw < 0) {
					m_cameraYaw += 2 * M_PI;
				}
				if (m_cameraYaw >= 2 * M_PI) {
					m_cameraYaw -= 2 * M_PI;
				}
				m_cameraPitch -= diffY * 0.00025;
				if (m_cameraPitch < -divededPi) {
					m_cameraPitch = -divededPi + 0.01;
				}
				if (m_cameraPitch > divededPi) {
					m_cameraPitch = divededPi - 0.01;
				}
				const float pitchCos = std::cos(m_cameraPitch);
				const float pitchSin = std::sin(m_cameraPitch);
				const float yawCos = std::cos(m_cameraYaw);
				const float yawSin = std::sin(m_cameraYaw);

				m_cameraForward = e3Dg::normalize(e3Dg::Vector3f(yawCos * pitchCos, pitchSin, yawSin * pitchCos));
			}

			SetCursorPos(screenCenterX, screenCenterY);
			m_cursorResetting = true;
		}
	};
	void OnKeyPress(const unsigned int& wParam, const bool& prevState, const bool& buttonState) override {
		m_buttonsStates[wParam] = buttonState;
	};

	void OnDraw() override {
		// TODO: Add rotatiion and move it to another class
		// A button
		if (m_buttonsStates[0x41]) {
			e3Dg::Vector3f right = e3Dg::crossProduct(e3Dg::normalize(e3Dg::Vector3f({0, 1, 0})), m_cameraForward);
			m_cameraPos = m_cameraPos + right * m_cameraSpeed * m_elapsedTime;
		}
		// D buton
		if (m_buttonsStates[0x44]) {
			e3Dg::Vector3f right = e3Dg::crossProduct(e3Dg::normalize(e3Dg::Vector3f({ 0, 1, 0 })), m_cameraForward);
			m_cameraPos = m_cameraPos - right * m_cameraSpeed * m_elapsedTime;
		}
		// W button
		if (m_buttonsStates[0x57]) {
			m_cameraPos = m_cameraPos - m_cameraForward * m_cameraSpeed * m_elapsedTime;
		}
		// S button
		if (m_buttonsStates[0x53]) {
			m_cameraPos = m_cameraPos + m_cameraForward * m_cameraSpeed * m_elapsedTime;
		}
		
		const int VERTICES_NUM = 36;
		// It is a sratch of future 3D edngine pipleine
		// TODO: I think all things like ignoreVertice or vertices itself can be moved to Polygon struct, so I need to create it
		bool ignoreVertice[VERTICES_NUM];
		for (int i = 0; i < VERTICES_NUM; ++i) ignoreVertice[i] = false;
		e3Dg::Vector3f vertices[VERTICES_NUM] = {
			// front
			{-0.5f, -0.5f, 0.0f }, { -0.5f, 0.5f, 0.0f }, { 0.5f, 0.5f, 0.0f },
			{-0.5f, -0.5f, 0.0f}, { 0.5f, 0.5f, 0.0f }, { 0.5f, -0.5f, 0.0f },
			// back
			{-0.5f, -0.5f, 1.0f}, { 0.5f, 0.5f, 1.0f }, { -0.5f, 0.5f, 1.0f },
			{-0.5f, -0.5f, 1.0f}, { 0.5f, -0.5f, 1.0f }, { 0.5f, 0.5f, 1.0f },
			// top
			{ -0.5f, 0.5f, 0.0f }, { -0.5f, 0.5f, 1.0f }, { 0.5f, 0.5f, 1.0f },
			{ -0.5f, 0.5f, 0.0f }, { 0.5f, 0.5f, 1.0f }, { 0.5f, 0.5f, 0.0f },
			// bottom
			{ -0.5f, -0.5f, 0.0f }, { 0.5f, -0.5f, 1.0f }, { -0.5f, -0.5f, 1.0f },
			{ -0.5f, -0.5f, 0.0f }, { 0.5f, -0.5f, 0.0f }, { 0.5f, -0.5f, 1.0f },
			// left
			{ -0.5f, -0.5f, 1.0f }, { -0.5f, 0.5f, 1.0f }, { -0.5f, 0.5f, 0.0f },
			{ -0.5f, -0.5f, 1.0f }, { -0.5f, 0.5f, 0.0f }, { -0.5f, -0.5f, 0.0f },
			// right
			{ 0.5f, -0.5f, 1.0f }, { 0.5f, 0.5f, 0.0f }, { 0.5f, 0.5f, 1.0f },
			{ 0.5f, -0.5f, 1.0f }, { 0.5f, -0.5f, 0.0f }, { 0.5f, 0.5f, 0.0f }
		};

		// Creating of the array in homogenous coordinates (it is stupid, but i just wanted to draw cube as fast as I can)
		e3Dg::Vector4f homogenVertices[VERTICES_NUM];
		for (int i = 0; i < VERTICES_NUM; ++i) {
			e3Dg::Vector3f vertex = vertices[i];
			homogenVertices[i] = { vertex[0], vertex[1], vertex[2], 1 };
		}

		e3Dg::Matrix4x4 modelMatrix = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		const float rotCos = std::cos(m_time);
		const float rotSin = std::sin(m_time);
		// Rotation over OY axi
		const e3Dg::Matrix4x4 yRotMat = {
			rotCos, 0, rotSin, 0,
			0, 1, 0, 0,
			-rotSin, 0, rotCos, 0,
			0, 0, 0, 1
		};
		// Rotation over OX axis
		const e3Dg::Matrix4x4 xRotMat = {
			1, 0, 0, 0,
			0, rotCos, -rotSin, 0,
			0, rotSin, rotCos, 0,
			0, 0, 0, 1
		};
		// Rotation over OZ axis
		const e3Dg::Matrix4x4 zRotMat = {
			rotCos, -rotSin, 0, 0,
			rotSin, rotCos, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		// Scaling and translation of the vertices
		const float scalingCoeff = 0.25;
		const e3Dg::Matrix4x4 scalingMat = {
			scalingCoeff, 0, 0, 0,
			0, scalingCoeff, 0, 0,
			0, 0, scalingCoeff, 0.0f,
			0, 0, 0, 1
		};
		modelMatrix = scalingMat * modelMatrix;

		// TODO: Undertand math under this part of the pipeline 
		// normals calculation
		int j = 0;
		for (int i = 0; i < VERTICES_NUM; i += 3) {
			homogenVertices[i] = modelMatrix * homogenVertices[i];
			homogenVertices[i + 1] = modelMatrix * homogenVertices[i + 1];
			homogenVertices[i + 2] = modelMatrix * homogenVertices[i + 2];
			const e3Dg::Vector4f aH = homogenVertices[i];
			const e3Dg::Vector4f bH = homogenVertices[i + 1];
			const e3Dg::Vector4f cH = homogenVertices[i + 2];
			// Calculation of the polygon normal (fV - firstVec, sV - secondVec)
			const e3Dg::Vector3f fV = e3Dg::vecToEuclid(bH - aH);
			const e3Dg::Vector3f sV = e3Dg::vecToEuclid(cH - aH);
			e3Dg::Vector3f normal = e3Dg::normalize(e3Dg::crossProduct(fV, sV));

			e3Dg::Vector3f vec = e3Dg::vecToEuclid(aH) - m_cameraPos;
			const float dotProduct = e3Dg::dotProduct(normal, vec);
			if (dotProduct > 0.0f) ignoreVertice[i] = true;
			++j;
		}

		e3Dg::Matrix4x4 resultMatrix = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		// cameraPos calculation
		// TODO: Derive lookAt matrix for different coordinate systems (left and right) in order to undestand mathematical background
		e3Dg::Matrix4x4 lookAtMatrix = lookAtCalc(m_cameraPos, m_cameraPos + m_cameraForward);
		resultMatrix = lookAtMatrix * resultMatrix;

		// Perspective projection calculation
		const float aspectRatio = (float)m_pixelsHNum / m_pixelsWNum;
		const float FoV = M_PI / 4;
		const float FoVValue = 1 / std::tan(FoV / 2);
		const float zNear = 0.1f;
		const float zFar = 100.0f;
		const float zDiff = zFar - zNear;
		// TODO: Derive this matrix for different coordinate systems (left and right) in order to undestand mathematical background
		const e3Dg::Matrix4x4 perspectiveProjMatrix = {
			aspectRatio * FoVValue, 0, 0, 0,
			0, FoVValue, 0, 0,
			0, 0, (zFar + zNear) / zDiff, - (2 * zNear * zFar) / zDiff,
			0, 0, 1, 0
		};
		resultMatrix = perspectiveProjMatrix * resultMatrix;

		for (int i = 0; i < VERTICES_NUM; ++i) {
			homogenVertices[i] = resultMatrix * homogenVertices[i];
			if (std::abs(homogenVertices[i].coord.w) >= 0.0001) {
				homogenVertices[i] = homogenVertices[i] / homogenVertices[i].coord.w;
				// TODO: Ignore vertice it is very bad realisation of clipping, it should be recreated
				if (std::abs(homogenVertices[i].coord.x) > 1 || std::abs(homogenVertices[i].coord.y) > 1) ignoreVertice[i] = true;
			}
			else {
				ignoreVertice[i] = true;
			}
		}

		// Here are drawing logic: I just sequently get 3 vertices from the array and look at the ignoreVertice data
		j = 0;
		for (int i = 0; i < VERTICES_NUM; i += 3) {
			if (ignoreVertice[i] || ignoreVertice[i + 1] || ignoreVertice[i + 2]) continue;
			const e3Dg::Vector4f aH = homogenVertices[i];
			const e3Dg::Vector4f bH = homogenVertices[i + 1];
			const e3Dg::Vector4f cH = homogenVertices[i + 2];
			// TODO: Move this to new method and think about how to pass data to the drawing functions
			const Pixel a = { aH[0] * m_pixelsWNum + m_pixelsWNum * 0.5, aH[1] * m_pixelsHNum + m_pixelsHNum * 0.5, { 255, 0, 0 } };
			const Pixel b = { bH[0] * m_pixelsWNum + m_pixelsWNum * 0.5, bH[1] * m_pixelsHNum + m_pixelsHNum * 0.5, { 0, 255, 0 } };
			const Pixel c = { cH[0] * m_pixelsWNum + m_pixelsWNum * 0.5, cH[1] * m_pixelsHNum + m_pixelsHNum * 0.5, { 0, 0, 255 } };
			InterpolatedTriangle(a, b, c);
			DrawTriangle(a, b, c);
			++j;
		}

		/*std::cout << "m_lastMouseX: " << m_lastMouseX << std::endl;
		std::cout << "m_lastMouseY: " << m_lastMouseY << std::endl;
		const Pixel first = { m_lastMouseX - 5, m_lastMouseY - 5, { 255, 0, 0 } };
		const Pixel second = { m_lastMouseX - 5, m_lastMouseY + 5, { 0, 255, 0 } };
		const Pixel third = { m_lastMouseX + 5, m_lastMouseY + 5, { 0, 0, 255 } };
		InterpolatedTriangle(first, second, third);
		DrawTriangle(first, second, third);
		const Pixel first1 = { m_lastMouseX - 5, m_lastMouseY - 5, { 255, 0, 0 } };
		const Pixel second1 = { m_lastMouseX + 5, m_lastMouseY + 5, { 0, 255, 0 } };
		const Pixel third1 = { m_lastMouseX + 5, m_lastMouseY - 5, { 0, 0, 255 } };
		InterpolatedTriangle(first1, second1, third1);
		DrawTriangle(first1, second1, third1);*/
	}
};


// TODO: I finally got the idea of the drawing pipeline. It shoul be splitted into 4 parts:
//       1. Translating from the model space to world space. This part will be done by a client with the using of the models
//          from the ModelsFactory. For this I'm need to create new classes: Model and ModelsFactory, that will work in the
//          cooperation with TexuresFactory by using link counting (for deleting of the unused models)
//       2. Translating from the world space to the camera space. For this I should create abstract BaseCamera class,
//          which will be using with the help of polymorphysm in the Engine3D class. It should have data about camera position,
//          vectors and matrices for camera translation, that will be used in this part of the pipeline
//       3. Translating from the camera space to clip space (by using projection matrix). This part of the application can be
//          written with default method and matrix for the one point perspective translation, but the method shoul be virtual
//          or something like this) and some lighting calculations (maybe I should ditinguish it into stage between this and 4).
//          Also I think there will be cool idea to create LightingSourcesFactory class that will hold all lighting sources
//          inherited from the base BaseLightningSource class
//       4. Translating from the clip space with NDC to screen space. On this part of the pipleni I should realise clipping and
//          translating result vertices to the screen coordinates. Maybe for this part I should change functions of drawing triangles
//          (but I don't sure in that) and add methods for drawing quads, lines and points.
//       I also shoud create auxilary structs for Polygon (in the form of triangle) and some sctructures for color blending
//       and lighting (lighting on the 3 part)
//       So, concluding all above first of all I should write Model and ModelsFactory class, after that start to create second
//       part of the pipeline with it BaseCamera class. Lighting should be the last think I need to care
