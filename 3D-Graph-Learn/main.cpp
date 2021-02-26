#include <iostream>
#include <chrono>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif
//#include "EngineWindow.h"
#include "EngineWindows.h"
#include "Engine3D.h"

// TODO: Here I have written a little part of the future 3D engine, so I belive I should move it into another file after
//       fixing all TODOs in the EngineWindow.h and EngineWindow.cpp
// TODO: Also I don't like namings, I will change them later

/*struct Vertex3f {
	float x, y, z;
};

struct Vertex4f {
	float x, y, z, w;
	bool drawable;

	void Display() {
		std::cout << "X: " << x << "; Y: " << y << "; Z: " << z << "; W: " << w << std::endl;
	}
};

struct PixelFloat {
	float x, y;
};

class Engine3D : public EngineWindow {
private:
	// TODO: Remove this thing, it is done just for testing
	Vertex3f m_lookTo;
public:
	Engine3D() {
		m_lookTo = { 0, 0, -1.0f };
	}
	virtual void OnUpdate(const float& elapsedTime, const float& time) override {
		Vertex3f vertices[36] = {
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
		Vertex4f homogenVertices[36];
		for (int i = 0; i < 36; ++i) {
			Vertex3f vertex = vertices[i];
			homogenVertices[i] = { vertex.x, vertex.y, vertex.z, 1, true };
		}

		const float rotCos = std::cos(time);
		const float rotSin = std::sin(time);
		// Rotation over OY axis
		for (int i = 0; i < 36; ++i) {
			Vertex4f vertex = homogenVertices[i];
			const float newX = vertex.x * rotCos + vertex.z * rotSin;
			const float newZ = -vertex.x * rotSin + vertex.z * rotCos;
			vertex.x = newX;
			vertex.z = newZ;
			homogenVertices[i] = vertex;
		}
		// Rotation over OX axis
		for (int i = 0; i < 36; ++i) {
			Vertex4f vertex = homogenVertices[i];
			const float newY = vertex.y * rotCos - vertex.z * rotSin;
			const float newZ = vertex.y * rotSin + vertex.z * rotCos;
			vertex.y = newY;
			vertex.z = newZ;
			homogenVertices[i] = vertex;
		}
		// Rotation over OZ axis
		for (int i = 0; i < 36; ++i) {
			Vertex4f vertex = homogenVertices[i];
			const float newX = vertex.x * rotCos - vertex.y * rotSin;
			const float newY = vertex.x * rotSin + vertex.y * rotCos;
			vertex.x = newX;
			vertex.y = newY;
			homogenVertices[i] = vertex;
		}

		const float scalingCoeff = 0.025;
		// Here I scale and translate vertices
		for (int i = 0; i < 36; ++i) {
			Vertex4f vertex = homogenVertices[i];
			vertex.x *= scalingCoeff;
			vertex.y *= scalingCoeff;
			vertex.z *= scalingCoeff;
			vertex.z -= 0.2f;
			homogenVertices[i] = vertex;
		}

		// Perspective projection calculation
		const float aspectRatio = (float)m_pixelsHNum / m_pixelsWNum;
		const float FoV = M_PI / 4;
		const float FoVValue = 1 / std::tan(FoV / 2);
		const float zNear = 0.1f;
		const float zFar = 100.0f;

		for (int i = 0; i < 36; ++i) {
			Vertex4f vertex = homogenVertices[i];
			vertex.x *= aspectRatio * FoVValue;
			vertex.y *= FoVValue;
			vertex.w *= vertex.z;
			vertex.z = vertex.z * (zFar + zNear) / (zFar - zNear) - (2 * zNear * zFar) / (zFar - zNear);
			if (vertex.w == 0) {
				vertex.drawable = false;
			} else {
				vertex.x /= vertex.w;
				vertex.y /= vertex.w;
				vertex.z /= vertex.w;
				vertex.w = 1;
			}
			homogenVertices[i] = vertex;
		}

		// Here are drawing logic: I just sequently get 3 vertices from the array and test normals of the polygons
		for (int i = 0; i < 36; i += 3) {
			const Vertex4f aH = homogenVertices[i];
			const Vertex4f bH = homogenVertices[i + 1];
			const Vertex4f cH = homogenVertices[i + 2];
			// Calculation of the polygon normal (fV - firstVec, sV - secondVec)
			const Vertex3f fV = { bH.x - aH.x, bH.y - aH.y, bH.z - aH.z };
			const Vertex3f sV = { cH.x - aH.x, cH.y - aH.y, cH.z - aH.z };
			const Vertex3f normal = { fV.y * fV.z - fV.z * sV.y, fV.z * sV.x - fV.x * sV.z, fV.x * sV.y - fV.y * sV.x };
			// Calculation of the dot product for testing normals
			const float dotProduct = m_lookTo.x * normal.x + m_lookTo.y * normal.y + m_lookTo.z * normal.z;
			if (dotProduct < 0) {
				const Pixel a = { aH.x * m_pixelsWNum + m_pixelsWNum * 0.5, aH.y * m_pixelsHNum + m_pixelsHNum * 0.5, { 255, 0, 0 } };
				const Pixel b = { bH.x * m_pixelsWNum + m_pixelsWNum * 0.5, bH.y * m_pixelsHNum + m_pixelsHNum * 0.5, { 0, 255, 0 } };
				const Pixel c = { cH.x * m_pixelsWNum + m_pixelsWNum * 0.5, cH.y * m_pixelsHNum + m_pixelsHNum * 0.5, { 0, 0, 255 } };
				FillTriangle(a, b, c);
				DrawTriangle(a, b, c);
			}
		}
	}
};*/

#include <array>

struct ass {
	int a, b;

	ass(int A, int B) {
		std::cout << this->a;
		a = A;
		b = B;

	}

	ass& operator=(std::initializer_list<int> list) {
		std::cout << "rar" << std::endl;
	}
};

int main() {
	MyGeo::Vector<3> vec = { "asd", 2 ,3 };
	vec.display();

	Engine3D engine;

	if (engine.CreateGameWindow(1000, 1000, 4, 4)) {
		engine.StartGame();
	}

	return 0;
}
