#include "Model3D.h"

Model3D::Model3D() {
	m_polygons = nullptr;
	m_polygonsSize = 0;
}

Model3D::~Model3D() {
	delete[] m_polygons;
}

void Model3D::LoadModel(const std::string& filePath) {
	std::vector<vgu::Vector3f> vertices;
	std::vector<vgu::Vector3f> normals;
	std::vector<vgu::Vector2f> textureVertices;
	std::vector<ModelFaceData> faces;
	std::ifstream file;
	file.exceptions(std::ifstream::badbit | std::ifstream::failbit | std::ifstream::eofbit);
	char junk;
	// TODO: Fix here the problem with the file closing, it is always throws EOF exception, this should be fixed
	// TODO: Also this method if VERY big and really bad formatted, I should think about refactoring this
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
					vgu::Vector2f texVec;
					sstr >> junk >> junk >> texVec.coord.x >> texVec.coord.y;
					textureVertices.push_back(texVec);
					break;
				}
				case 'n': {
					vgu::Vector3f normalVec;
					sstr >> junk >> junk >> normalVec.coord.x >> normalVec.coord.y >> normalVec.coord.z;
					normals.push_back(normalVec);
					break;
				}
				default: {
					vgu::Vector3f vertVec;
					sstr >> junk >> vertVec.coord.x >> vertVec.coord.y >> vertVec.coord.z;
					vertices.push_back(vertVec);
				}
				}
			}
			if (line[0] == 'f') {
				sstr >> junk;
				std::vector<VertexData> verticesData;
				while (sstr.peek() != -1) {
					VertexData data = { 0, 0, 0 };
					sstr >> data.vertice;
					// Test if there is only vertex data in the face element
					if (sstr.peek() == ' ' || sstr.peek() == -1) continue;
					sstr >> junk;
					// Test if there is no texture part
					if (sstr.peek() == '/') {
						sstr >> junk >> data.normal;
					}
					else {
						sstr >> data.texture;
						// Test if there is no normals part
						if (sstr.peek() == ' ' || sstr.peek() == -1) continue;
						sstr >> junk >> data.normal;
					}
					data.vertice -= 1;
					data.texture -= 1;
					data.normal -= 1;
					verticesData.push_back(data);
				}
				// Making triangles from face if needed
				if (verticesData.size() >= 3) {
					for (int i = 0; i < verticesData.size() - 2; ++i) {
						faces.push_back({ verticesData[0], verticesData[(long long)i + 1], verticesData[(long long)i + 2] });
					}
				}
			}
		}
	}
	catch (std::ifstream::failure e) {
		if (!file.eof()) {
			std::cout << "MODEL_LOAD::FILE_ERROR::" << e.what() << std::endl;
		}
		else {
			file.close();
			m_polygonsSize = faces.size();
			// TODO: Rewrite this, it is terrible
			m_polygons = new Polygon3P[m_polygonsSize];
			for (int i = 0; i < m_polygonsSize; ++i) {
				vgu::Vector4f first = vgu::vecToHomogen(-vertices[faces[i].data[0].vertice]);
				vgu::Vector4f second = vgu::vecToHomogen(-vertices[faces[i].data[1].vertice]);
				vgu::Vector4f third = vgu::vecToHomogen(-vertices[faces[i].data[2].vertice]);
				m_polygons[i] = { first, second, third };
				m_polygons[i].CalculateNormal();
			}
		}
	}
}

Polygon3P* Model3D::GetPolygons() {
	Polygon3P* newPolygons = new Polygon3P[m_polygonsSize];
	std::copy(m_polygons, m_polygons + m_polygonsSize, newPolygons);
	return newPolygons;
}

size_t Model3D::GetPolygonsSize() const {
	return m_polygonsSize;
}
