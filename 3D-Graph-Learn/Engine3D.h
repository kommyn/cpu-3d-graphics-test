#pragma once

#include <cmath>

#include "EngineWindows.h"
#include "TexturesFactory.h"
#include "Matrix.h"
#include "GraphUtils.h"
#include "Camera.h"

class Engine3D : public EngineWindows
{
private:
	void InterpolatedHalfTriangle(const Pixel& a, const Pixel& b, float* coeffs);
	void TexturedHalfTriangle(const Pixel& a, const Pixel& b);
protected:
	TexturesFactory m_texturesFact;
	bool m_buttonsStates[255];

	// TODO: Remove this class out of here, it is just temporary solution
	Camera m_camera;
public:
	Engine3D();
	~Engine3D() = default;

	void DrawLine(Pixel a, Pixel b);
	void DrawTriangle(const Pixel& a, const Pixel& b, const Pixel& c);
	void InterpolatedTriangle(Pixel a, Pixel b, Pixel c);
	void TexturedTriangle(Pixel a, Pixel b, Pixel c, std::string filePath);
	void VertexPipe();

	void OnMouseMove(const int& xPos, const int& yPos) override {
		Point screenCenter = GetScreenCoordinate({ (m_screenWidth) / 2, m_screenHeight / 2 });
		int diffX = xPos - screenCenter.x;
		int diffY = yPos - screenCenter.y;
		if (diffX == 0 && diffY == 0) return;
		m_camera.RecalculateAngles(diffX, diffY);
	};

	void OnKeyPress(const unsigned int& wParam, const bool& prevState, const bool& buttonState) override {
		m_buttonsStates[wParam] = buttonState;
	};

	void OnDraw() override {
		m_camera.RecalculatePosition({ m_buttonsStates[0x41], m_buttonsStates[0x44], m_buttonsStates[0x57], m_buttonsStates[0x53] }, m_elapsedTime);
		
		const int VERTICES_NUM = 36;
		// It is a sratch of future 3D edngine pipleine
		// TODO: I think all things like ignoreVertice or vertices itself can be moved to Polygon struct, so I need to create it
		bool ignoreVertice[VERTICES_NUM];
		for (int i = 0; i < VERTICES_NUM; ++i) ignoreVertice[i] = false;
		vgu::Vector3f vertices[VERTICES_NUM] = {
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
		vgu::Vector4f homogenVertices[VERTICES_NUM];
		for (int i = 0; i < VERTICES_NUM; ++i) {
			vgu::Vector3f vertex = vertices[i];
			homogenVertices[i] = { vertex[0], vertex[1], vertex[2], 1 };
		}

		vgu::Matrix4x4 modelMatrix = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		const float rotCos = std::cos(m_time);
		const float rotSin = std::sin(m_time);
		// Rotation over OY axi
		const vgu::Matrix4x4 yRotMat = {
			rotCos, 0, rotSin, 0,
			0, 1, 0, 0,
			-rotSin, 0, rotCos, 0,
			0, 0, 0, 1
		};
		// Rotation over OX axis
		const vgu::Matrix4x4 xRotMat = {
			1, 0, 0, 0,
			0, rotCos, -rotSin, 0,
			0, rotSin, rotCos, 0,
			0, 0, 0, 1
		};
		// Rotation over OZ axis
		const vgu::Matrix4x4 zRotMat = {
			rotCos, -rotSin, 0, 0,
			rotSin, rotCos, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		// Scaling and translation of the vertices
		const float scalingCoeff = 0.25;
		const vgu::Matrix4x4 scalingMat = {
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
			const vgu::Vector4f aH = homogenVertices[i];
			const vgu::Vector4f bH = homogenVertices[i + 1];
			const vgu::Vector4f cH = homogenVertices[i + 2];
			// Calculation of the polygon normal (fV - firstVec, sV - secondVec)
			const vgu::Vector3f fV = vgu::vecToEuclid(bH - aH);
			const vgu::Vector3f sV = vgu::vecToEuclid(cH - aH);
			vgu::Vector3f normal = vgu::normalize(vgu::crossProduct(fV, sV));

			vgu::Vector3f vec = vgu::vecToEuclid(aH) - m_camera.GetPos();
			const float dotProduct = vgu::dotProduct(normal, vec);
			if (dotProduct > 0.0f) ignoreVertice[i] = true;
			++j;
		}

		vgu::Matrix4x4 resultMatrix = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		// cameraPos calculation
		// TODO: Derive lookAt matrix for different coordinate systems (left and right) in order to undestand mathematical background
		vgu::Matrix4x4 lookAtMatrix = m_camera.GetLookAtMatrix();
		resultMatrix = lookAtMatrix * resultMatrix;

		// Perspective projection calculation
		const float aspectRatio = (float)m_pixelsHNum / m_pixelsWNum;
		const float FoV = M_PI / 4;
		const float FoVValue = 1 / std::tan(FoV / 2);
		const float zNear = 0.1f;
		const float zFar = 100.0f;
		const float zDiff = zFar - zNear;
		// TODO: Derive this matrix for different coordinate systems (left and right) in order to undestand mathematical background
		const vgu::Matrix4x4 perspectiveProjMatrix = {
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
				if (std::abs(homogenVertices[i].coord.x) > 1 || std::abs(homogenVertices[i].coord.y) > 1 || std::abs(homogenVertices[i].coord.z) > 1) ignoreVertice[i] = true;
			}
			else {
				ignoreVertice[i] = true;
			}
		}

		// Here are drawing logic: I just sequently get 3 vertices from the array and look at the ignoreVertice data
		j = 0;
		for (int i = 0; i < VERTICES_NUM; i += 3) {
			if (ignoreVertice[i] || ignoreVertice[i + 1] || ignoreVertice[i + 2]) continue;
			const vgu::Vector4f aH = homogenVertices[i];
			const vgu::Vector4f bH = homogenVertices[i + 1];
			const vgu::Vector4f cH = homogenVertices[i + 2];
			// TODO: Move this to new method and think about how to pass data to the drawing functions
			const Pixel a = { aH[0] * m_pixelsWNum + m_pixelsWNum * 0.5, aH[1] * m_pixelsHNum + m_pixelsHNum * 0.5, { 255, 0, 0 } };
			const Pixel b = { bH[0] * m_pixelsWNum + m_pixelsWNum * 0.5, bH[1] * m_pixelsHNum + m_pixelsHNum * 0.5, { 0, 255, 0 } };
			const Pixel c = { cH[0] * m_pixelsWNum + m_pixelsWNum * 0.5, cH[1] * m_pixelsHNum + m_pixelsHNum * 0.5, { 0, 0, 255 } };
			InterpolatedTriangle(a, b, c);
			DrawTriangle(a, b, c);
			++j;
		}

		/*const Pixel first = { m_lastMouseX - 5, m_lastMouseY - 5, { 255, 255, 255 } };
		const Pixel second = { m_lastMouseX - 5, m_lastMouseY + 5, { 255, 255, 255 } };
		const Pixel third = { m_lastMouseX + 5, m_lastMouseY + 5, { 255, 255, 255 } };
		InterpolatedTriangle(first, second, third);
		DrawTriangle(first, second, third);
		const Pixel first1 = { m_lastMouseX - 5, m_lastMouseY - 5, { 255, 255, 255 } };
		const Pixel second1 = { m_lastMouseX + 5, m_lastMouseY + 5, { 255, 255, 255 } };
		const Pixel third1 = { m_lastMouseX + 5, m_lastMouseY - 5, { 255, 255, 255 } };
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
