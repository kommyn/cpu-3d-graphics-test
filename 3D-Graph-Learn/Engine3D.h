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

class Engine3D : public EngineWindows
{
private:
	void InterpolatedHalfTriangle(const Pixel& a, const Pixel& b, float* coeffs);
	void TexturedHalfTriangle(const Pixel& a, const Pixel& b);
protected:
	TexturesFactory m_texturesFact;
	// TODO: Move camera logic to another class, this class should only has functionality of 3D pipeline usage
	e3Dg::Vector3f m_lookTo;
public:
	Engine3D();
	~Engine3D() = default;

	void DrawLine(Pixel a, Pixel b);
	void DrawTriangle(const Pixel& a, const Pixel& b, const Pixel& c);
	void InterpolatedTriangle(Pixel a, Pixel b, Pixel c);
	void TexturedTriangle(Pixel a, Pixel b, Pixel c, std::string filePath);
	void VertexPipe();

	void OnDraw() override {
		/*RGBAColor color = { 255, 0, 0, 255 };
		Pixel a = { 100, 150, { 255, 0, 0, 255 } };
		Pixel b = { 100, 125, { 0, 255, 0, 255 } };
		Pixel c = { 200, 200, { 0, 0, 255, 255 } };
		//DrawLine(a, b);
		//InterpolatedTriangle(a, b, c);
		TexturedTriangle(a, b, c, "asd");*/
		

		// It is a sratch of future 3D edngine pipleine
		e3Dg::Vector3f vertices[36] = {
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
		e3Dg::Vector4f homogenVertices[36];
		for (int i = 0; i < 36; ++i) {
			e3Dg::Vector3f vertex = vertices[i];
			homogenVertices[i] = { vertex[0], vertex[1], vertex[2], 1 };
		}

		const float rotCos = std::cos(m_time);
		const float rotSin = std::sin(m_time);
		// Rotation over OY axi
		const e3Dg::Matrix4x4 yRotmat = {
			rotCos, 0, rotSin, 0,
			0, 1, 0, 0,
			-rotSin, 0, rotCos, 0,
			0, 0, 0, 1
		};
		for (int i = 0; i < 36; ++i) {
			homogenVertices[i] = homogenVertices[i] * yRotmat;
		}
		// Rotation over OX axis
		const e3Dg::Matrix4x4 xRotMat = {
			1, 0, 0, 0,
			0, rotCos, -rotSin, 0,
			0, rotSin, rotCos, 0,
			0, 0, 0, 1
		};
		for (int i = 0; i < 36; ++i) {
			e3Dg::Vector4f vertex = homogenVertices[i];
			homogenVertices[i] = homogenVertices[i] * xRotMat;
		}
		// Rotation over OZ axis
		const e3Dg::Matrix4x4 zRotMat = {
			rotCos, -rotSin, 0, 0,
			rotSin, rotCos, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		for (int i = 0; i < 36; ++i) {
			homogenVertices[i] = homogenVertices[i] * zRotMat;
		}

		const float scalingCoeff = 0.025;
		// Here I scale and translate vertices
		const e3Dg::Matrix4x4 scalingMat = {
			scalingCoeff, 0, 0, 0,
			0, scalingCoeff, 0, 0,
			0, 0, scalingCoeff, 0,
			0, 0, 0, 1
		};
		for (int i = 0; i < 36; ++i) {
			homogenVertices[i] = homogenVertices[i] * scalingMat;
			homogenVertices[i].coord.z -= 0.2f;
		}

		// Perspective projection calculation
		const float aspectRatio = (float)m_pixelsHNum / m_pixelsWNum;
		const float FoV = M_PI / 4;
		const float FoVValue = 1 / std::tan(FoV / 2);
		const float zNear = 0.1f;
		const float zFar = 100.0f;
		const float zDiff = zFar - zNear;
		const e3Dg::Matrix4x4 perspectiveProjMatrix = {
			aspectRatio * FoVValue, 0, 0, 0,
			0, FoVValue, 0, 0,
			0, 0, (zFar + zNear) / zDiff, 1,
			0, 0, - (2 * zNear * zFar) / zDiff, 0
		};

		for (int i = 0; i < 36; ++i) {
			homogenVertices[i] = homogenVertices[i] * perspectiveProjMatrix;
			if (homogenVertices[i].coord.w != 0) {
				homogenVertices[i] = homogenVertices[i] / homogenVertices[i].coord.w;
			}
		}

		// Here are drawing logic: I just sequently get 3 vertices from the array and test normals of the polygons
		for (int i = 0; i < 36; i += 3) {
			const e3Dg::Vector4f aH = homogenVertices[i];
			const e3Dg::Vector4f bH = homogenVertices[i + 1];
			const e3Dg::Vector4f cH = homogenVertices[i + 2];
			// Calculation of the polygon normal (fV - firstVec, sV - secondVec)
			const e3Dg::Vector3f fV = e3Dg::vecToEuclid(bH - aH);
			const e3Dg::Vector3f sV = e3Dg::vecToEuclid(cH - aH);
			const e3Dg::Vector3f normal = e3Dg::crossProduct(fV, sV);
			// Calculation of the dot product for testing normals
			const float dotProduct = e3Dg::dotProduct(m_lookTo, normal);
			if (dotProduct < 0) {
				// TODO: Move this to new method and think about how to pass data to the drawing functions
				const Pixel a = { aH[0] * m_pixelsWNum + m_pixelsWNum * 0.5, aH[1] * m_pixelsHNum + m_pixelsHNum * 0.5, { 255, 0, 0 } };
				const Pixel b = { bH[0] * m_pixelsWNum + m_pixelsWNum * 0.5, bH[1] * m_pixelsHNum + m_pixelsHNum * 0.5, { 0, 255, 0 } };
				const Pixel c = { cH[0] * m_pixelsWNum + m_pixelsWNum * 0.5, cH[1] * m_pixelsHNum + m_pixelsHNum * 0.5, { 0, 0, 255 } };
				InterpolatedTriangle(a, b, c);
				DrawTriangle(a, b, c);
			}
		}
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
