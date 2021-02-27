#include <iostream>
#include <chrono>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Engine3D.h"

class Model3D {
private:
	unsigned int verticesNum;
	e3Dg::Vector3f* vertices;
	e3Dg::Vector3f* textureCoords;
public:

};


int main() {
	e3Dg::Vector<2> vec = { 1, 2 };

	Engine3D engine;

	if (engine.CreateGameWindow(1000, 1000, 1, 1)) {
		engine.StartGame();
	}

	return 0;
}
