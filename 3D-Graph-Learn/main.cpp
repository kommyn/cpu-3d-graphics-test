#include <iostream>
#include <chrono>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Engine3D.h"

#include <vector>
#include <fstream>
#include <sstream>

class Model3D {
private:
	std::vector<e3Dg::Vector3f> m_vertices;
	std::vector<e3Dg::Vector2f> m_textureCoords;
public:
	Model3D(const std::string& filePath) {
		std::ifstream file;
		file.exceptions(std::ifstream::badbit | std::ifstream::failbit | std::ifstream::eofbit);
		char junk;
		try {
			file.open(filePath.c_str());
			char line[128];
			while (file.getline(line, 128)) {
				//std::cout << line << std::endl;
				std::stringstream sstr;
				sstr << line;
				if (line[0] == 'v') {
					switch (line[1]) {
					case 't': {
						e3Dg::Vector2f texVec;
						sstr >> junk >> junk >> texVec.coord.x >> texVec.coord.y;
						m_textureCoords.push_back(texVec);
						break;
					}
					case 'n':
						break;
					default: {
						e3Dg::Vector3f vertVec;
						sstr >> junk >> vertVec.coord.x >> vertVec.coord.y >> vertVec.coord.z;
						m_vertices.push_back(vertVec);
					}
					}
				}
			}
			std::cout << "End of reading" << std::endl;
			file.close();
		}
		catch (std::ifstream::failure e) {
			if (!file.eof()) {
				std::cout << "MODEL_LOAD::FILE_ERROR::" << e.what() << std::endl;
			}
		}
	}

	unsigned int vertSize() const {
		return m_vertices.size();
	}

	unsigned int textSize() const {
		return m_textureCoords.size();
	}

	e3Dg::Vector3f getVertex(const unsigned int& index) const {
		return m_vertices[index];
	}

	e3Dg::Vector2f getTextureCoords(const unsigned int& index) const {
		return m_textureCoords[index];
	}
};

int main() {
	//Model3D model("models/Izba/OBJ/Farmhouse OBJ.obj");

	Engine3D engine;

	if (engine.CreateGameWindow(1000, 1000, 1, 1)) {
		engine.StartGame();
	}

	return 0;
}
