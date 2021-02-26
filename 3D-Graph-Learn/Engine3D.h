#pragma once

#include <cmath>

#include "EngineWindows.h"
#include "TexturesFactory.h"
#include "Matrix.h"

struct Polygon3Vert {
	MyGeo::Vector3f coords[3];
	MyGeo::Vector2f textCoords[3];
};

class Model3D {
private:
	unsigned int verticesNum;
	MyGeo::Vector3f* vertices;
	MyGeo::Vector3f* textureCoords;
public:

};

class Engine3D : public EngineWindows
{
private:
	void InterpolatedHalfTriangle(const Pixel& a, const Pixel& b, float* coeffs);
	void TexturedHalfTriangle(const Pixel& a, const Pixel& b);
protected:
	TexturesFactory m_texturesFact;
	// TODO: Move camera logic to another class, this class should only has functionality of 3D pipeline usage
	MyGeo::Vector3f m_lookTo;
public:
	Engine3D();
	~Engine3D() = default;

	void DrawLine(Pixel a, Pixel b);
	void DrawTriangle(const Pixel& a, const Pixel& b, const Pixel& c);
	void InterpolatedTriangle(Pixel a, Pixel b, Pixel c);
	void TexturedTriangle(Pixel a, Pixel b, Pixel c, std::string filePath);
	void VertexPipe();

	void OnDraw() override {
		RGBAColor color = { 255, 0, 0, 255 };
		Pixel a = { 100, 150, { 255, 0, 0, 255 } };
		Pixel b = { 100, 125, { 0, 255, 0, 255 } };
		Pixel c = { 200, 200, { 0, 0, 255, 255 } };
		//DrawLine(a, b);
		//InterpolatedTriangle(a, b, c);
		TexturedTriangle(a, b, c, "asd");
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
