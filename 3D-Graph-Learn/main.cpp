#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Engine3D.h"

int main() {
	Engine3D engine;
	//C:\Users\user\source\repos\3D-Graph-Learn\3D-Graph-Learn
	/*try {
		Texture texture("tiles.png");
	}
	catch (const std::exception& error) {
		std::cout << "ERROR::" << error.what() << std::endl;
	}*/

	if (engine.CreateGameWindow(1000, 1000, 1, 1)) {
		engine.StartGame();
	}

	return 0;
}
