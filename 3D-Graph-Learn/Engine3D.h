#pragma once

#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>

#include "EngineWindows.h"
#include "TexturesFactory.h"
#include "Camera.h"
#include "Model3D.h"
#include "ModelsFactory.h"


// It is list of paths I use for testing
//m_model.LoadModel("models/cottage/OBJ/cottage_obj.obj");
//m_model.LoadModel("models/Izba/OBJ/Farmhouse OBJ.obj");
//m_model.LoadModel("models/sample/OBJ/model.obj");
//m_model.LoadModel("models/sample/OBJ/model-hr.obj");

// TODO: Add method for drawing textured triangle
// TODO: I should clean code and begin writing clipping algorythm
class Engine3D : public EngineWindows
{
private:
	void InterpolatedHalfTriangle(const Pixel& a, const Pixel& b, float* coeffs);
	//void TexturedHalfTriangle(const Pixel& a, const Pixel& b);
	// TODO: Fix this function args, they are too many and chaotic, I should fix this
	// TODO: Also I don't like the way I am passing here texture, I shold rethink it
	void FillHalfTriangle(const Pixel& a, const Pixel& b, const float coeffs[3], const double coeefsD[3], Texture* texture, const double coeffsT[6]);
protected:
	TexturesFactory m_texturesFact;
	ModelsFactory m_modelsFact;
	bool m_buttonsStates[255];
	double* m_zBuffer;
	vgu::Matrix4x4 m_projectionMatrix;

	// TODO: Remove this class out of here, it is just temporary solution
	Camera m_camera;
public:
	Engine3D();
	~Engine3D() override {
		if (!m_zBuffer) delete[] m_zBuffer;
	}

	// !!!!!!!!!!!!!!!IMPORTANT!!!!!!!!!!!!!!!
	// TODO: I need to add opprtunity to set filled triangle color (maybe in the Polygon3P class)
	// !!!!!!!!!!!!!!!IMPORTANT!!!!!!!!!!!!!!!
	void DrawLine(Pixel a, Pixel b, const double coeffs[3]);
	void DrawTriangle(const Polygon3P& polygon);
	//void InterpolatedTriangle(Pixel a, Pixel b, Pixel c);
	void TexturedTriangle(Pixel a, Pixel b, Pixel c, std::string filePath);
	void FillTriangle(const Polygon3P& polygon);

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

	// TODO: Split this method logic onto several methods that will be used for different operations:
	//       1. Z-buffer resetting (with ability of enabling/disabling this)
	//       2. Getting polygons (it is much more complex problem, I should think about ways of keeping calculated polygons data, for now
	//          I lean towards using of vector for all models and linked list of polygons in every model)
	//       3. Model matrix method for every model
	//       4. Normals calculation (I need to give oppertunity to enable/disable this)
	//       5. Getting camera matrix !!!DONE!!!
	//       6. Projection matrix calculation (maybe for this there will be a good idea to create abstract class of projection matrix
	//          with memoization and recalculation of matrix itself only on some events defined by client) and perspectivi division !!!DONE!!!
	//       7. Drawing logic itself
	//       Every model or polygon (I don't know what will be better choice) must be passed as argument in all this functions
	void OnDraw() override {
		size_t size = static_cast<size_t>(m_pixelsWNum) * static_cast<size_t>(m_pixelsHNum);
		m_zBuffer = new double[size];
		std::fill(m_zBuffer, m_zBuffer + size, std::numeric_limits<double>::infinity());

		m_camera.RecalculatePosition({ m_buttonsStates[0x41], m_buttonsStates[0x44], m_buttonsStates[0x57], m_buttonsStates[0x53] }, m_elapsedTime);
		
		// It is a scratch of future 3D edngine pipleine
		// TODO: Remove this, it is just a temporary solution
		auto model = m_modelsFact.LoadModel("models/Izba/OBJ/Farmhouse OBJ.obj");
		// auto model = m_modelsFact.LoadModel("models/sample/OBJ/model-hr.obj");
		const size_t POLYGONS_NUM = model->GetPolygonsSize();
		Polygon3P* polygons = model->GetPolygons();
		
		vgu::Matrix4x4 modelMatrix = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		// Scaling and translation of the vertices
		const float scalingCoeff = 0.025;
		const vgu::Matrix4x4 scalingMat = {
			scalingCoeff, 0, 0, 0,
			0, scalingCoeff, 0, 0,
			0, 0, scalingCoeff, 0,
			0, 0, 0, 1
		};
		const vgu::Matrix4x4 rotMat = {
			1, 0, 0, 0,
			0, 0, 1, 0,
			0, -1, 0, 0,
			0, 0, 0, 1
		};
		modelMatrix = scalingMat * modelMatrix;

		// TODO: Undertand math under this part of the pipeline
		// normals calculation
		for (size_t i = 0; i < POLYGONS_NUM; ++i) {
			polygons[i] = modelMatrix * polygons[i];
			polygons[i].m_normal = vgu::normalize(vgu::vecToEuclid(modelMatrix * vgu::vecToHomogen(polygons[i].m_normal)));
			const vgu::Vector3f vec = vgu::vecToEuclid(polygons[i].m_first) - m_camera.GetPos();
			const float dotProduct = vgu::dotProduct(polygons[i].m_normal, vec);
			if (dotProduct < 0) polygons[i].ignorePolygon = true;
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
		const float FoV = static_cast<float>(M_PI / 4);
		const float FoVValue = 1 / std::tan(FoV / 2);
		const float zNear = 0.01f;
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

		for (size_t i = 0; i < POLYGONS_NUM; ++i) {
			if (polygons[i].ignorePolygon) continue;
			polygons[i] = resultMatrix * polygons[i];
			polygons[i].PerspectiveDivision();
		}

		// Drawing logic itself
		for (size_t i = 0; i < POLYGONS_NUM; ++i) {
			if (polygons[i].ignorePolygon) continue;
			//DrawTriangle(polygons[i]);
			FillTriangle(polygons[i]);
		}

		// TODO: Remove this, it is temporary solution just to see is everything working well
		delete[] polygons;
		delete[] m_zBuffer;
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
//       4. Translating from the clip space with NDC to screen space. On this part of the pipelene I should realise clipping and
//          translating result vertices to the screen coordinates. Maybe for this part I should change functions of drawing triangles
//          (but I don't sure in that) and add methods for drawing quads, lines and points.
//       I also shoud create auxilary structs for Polygon (in the form of triangle) and some sctructures for color blending
//       and lighting (lighting on the 3 part)
//       So, concluding all above first of all I should write Model and ModelsFactory class, after that start to create second
//       part of the pipeline with it BaseCamera class. Lighting should be the last think I need to care
