#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Engine3D.h"

int main() {
	Engine3D engine;

	if (engine.CreateGameWindow(1000, 1000, 1, 1)) {
		engine.StartGame();
	}

	return 0;
}
