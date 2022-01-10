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

	/*std::list<int> rar;

	int i = 1;

	rar.push_back(i);
	i += 1;
	rar.push_back(i);

	std::list<int>::iterator rer = rar.begin();

	for (rer = rar.begin(); rer != rar.end(); ++rer) {
		std::cout << "I value: " << i << std::endl;
		std::cout << "Ra: " << *rer << std::endl;
		std::cout << "Begin: " << &(*rar.begin()) << std::endl;
		std::cout << "End: " << &(*rar.end())<< std::endl;
		std::cout << std::endl;
		i += 1;
		if (i < 10) {
			rar.pop_front();
			rar.push_back(i);
		}
		if (i >= 13) break;
	}*/

	return 0;
}
