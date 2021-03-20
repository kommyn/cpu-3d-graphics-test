#pragma once

#include <cmath>

#include "EngineWindows.h"
#include "TexturesFactory.h"
#include "Matrix.h"
#include "GraphUtils.h"
#include "Camera.h"

// TODO: Think about creating template class from this and moving all realisation into .cpp file
// TODO: Also I think I should create new utils file, call it like GraphUtils3D and move all 3D calc (including polygon class or struct) logic into it
// TODO: Create util functions for matrix base transformations (rotation, translation, scaling and something else)
struct Polygon3P {
	vgu::Vector4f first, second, third;
	bool ignorePolygon;

	friend Polygon3P operator*(const vgu::Matrix4x4& matrix, Polygon3P polygon) {
		polygon.first = matrix * polygon.first;
		polygon.second = matrix * polygon.second;
		polygon.third = matrix * polygon.third;
		return polygon;
	}

	vgu::Vector3f GetNormal() {
		const vgu::Vector3f fV = vgu::vecToEuclid(second - first);
		const vgu::Vector3f sV = vgu::vecToEuclid(third - first);
		return vgu::crossProduct(fV, sV);
	}

	void PerspectiveDivision() {
		if (ignorePolygon) return;
		vgu::Vector4f points[3] = { first, second, third };
		for (int i = 0; i < 3; ++i) {
			if (points[i].coord.w > 0.0001) {
				points[i] /= points[i].coord.w;
				if (std::abs(points[i].coord.x) > 1 ||
					std::abs(points[i].coord.y) > 1 ||
					std::abs(points[i].coord.z) > 1) {
					ignorePolygon = true;
					return;
				}
			}
			else {
				ignorePolygon = true;
				return;
			}
		}
		first = points[0];
		second = points[1];
		third = points[2];
	}
};

// TODO: Add method for drawing filled triangle, and think about opportunity of changing all drawig methods parameters to something like Polygon3P or 
//       something like this
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
		
		// It is a scratch of future 3D edngine pipleine
		const unsigned int POLYGONS_NUM = 12;
		Polygon3P polygons[12] = {
			// front
			{{ -0.5f, -0.5f, 0.0f, 1.0f }, { -0.5f, 0.5f, 0.0f, 1.0f }, { 0.5f, 0.5f, 0.0f, 1.0f }},
			{{ -0.5f, -0.5f, 0.0f, 1.0f }, { 0.5f, 0.5f, 0.0f, 1.0f }, { 0.5f, -0.5f, 0.0f, 1.0f } },
			// back
			{{ -0.5f, -0.5f, 1.0f, 1.0f }, { 0.5f, 0.5f, 1.0f, 1.0f }, { -0.5f, 0.5f, 1.0f, 1.0f }},
			{{ -0.5f, -0.5f, 1.0f, 1.0f }, { 0.5f, -0.5f, 1.0f, 1.0f }, { 0.5f, 0.5f, 1.0f, 1.0f }},
			// top
			{{ -0.5f, 0.5f, 0.0f, 1.0f }, { -0.5f, 0.5f, 1.0f, 1.0f }, { 0.5f, 0.5f, 1.0f, 1.0f }},
			{{ -0.5f, 0.5f, 0.0f, 1.0f }, { 0.5f, 0.5f, 1.0f, 1.0f }, { 0.5f, 0.5f, 0.0f, 1.0f }},
			// bottom
			{{ -0.5f, -0.5f, 0.0f, 1.0f }, { 0.5f, -0.5f, 1.0f, 1.0f }, { -0.5f, -0.5f, 1.0f, 1.0f }},
			{{ -0.5f, -0.5f, 0.0f, 1.0f }, { 0.5f, -0.5f, 0.0f, 1.0f }, { 0.5f, -0.5f, 1.0f, 1.0f }},
			// left
			{{ -0.5f, -0.5f, 1.0f, 1.0f }, { -0.5f, 0.5f, 1.0f, 1.0f }, { -0.5f, 0.5f, 0.0f, 1.0f }},
			{{ -0.5f, -0.5f, 1.0f, 1.0f }, { -0.5f, 0.5f, 0.0f, 1.0f }, { -0.5f, -0.5f, 0.0f, 1.0f }},
			// right
			{{ 0.5f, -0.5f, 1.0f, 1.0f }, { 0.5f, 0.5f, 0.0f, 1.0f }, { 0.5f, 0.5f, 1.0f, 1.0f }},
			{{ 0.5f, -0.5f, 1.0f, 1.0f }, { 0.5f, -0.5f, 0.0f, 1.0f }, { 0.5f, 0.5f, 0.0f, 1.0f }}
		};

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
		for (int i = 0; i < POLYGONS_NUM; ++i) {
			polygons[i] = modelMatrix * polygons[i];
			const vgu::Vector3f normal = polygons[i].GetNormal();
			const vgu::Vector3f vec = vgu::vecToEuclid(polygons[i].first) - m_camera.GetPos();
			const float dotProduct = vgu::dotProduct(normal, vec);
			if (dotProduct > 0) polygons[i].ignorePolygon = true;
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

		for (int i = 0; i < POLYGONS_NUM; ++i) {
			if (polygons[i].ignorePolygon) continue;
			polygons[i] = resultMatrix * polygons[i];
			polygons[i].PerspectiveDivision();
		}

		// Drawing logic itself
		// TODO: Do something with converting polygon data to pixels (maybe it is a good idea to write method for this in this class)
		for (int i = 0; i < POLYGONS_NUM; ++i) {
			if (polygons[i].ignorePolygon) continue;

			const Pixel a = { polygons[i].first[0] * m_pixelsWNum + m_pixelsWNum * 0.5, polygons[i].first[1] * m_pixelsHNum + m_pixelsHNum * 0.5, { 255, 0, 0 } };
			const Pixel b = { polygons[i].second[0] * m_pixelsWNum + m_pixelsWNum * 0.5, polygons[i].second[1] * m_pixelsHNum + m_pixelsHNum * 0.5, { 0, 255, 0 } };
			const Pixel c = { polygons[i].third[0] * m_pixelsWNum + m_pixelsWNum * 0.5, polygons[i].third[1] * m_pixelsHNum + m_pixelsHNum * 0.5, { 0, 0, 255 } };
			InterpolatedTriangle(a, b, c);
			DrawTriangle(a, b, c);
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
