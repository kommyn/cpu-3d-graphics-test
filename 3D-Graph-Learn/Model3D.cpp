#include "Model3D.h"

Model3D::Model3D() {
	m_polygons = nullptr;
	m_polygonsSize = 0;
}

Model3D::~Model3D() {
	delete[] m_polygons;
}

std::map<std::string, MtlData> Model3D::GetMtlData(const std::string& mtlPath) {
	std::regex secReg("(.*\/)OBJ\/");
	std::smatch result;
	std::regex_search(mtlPath, result, secReg);
	std::string folder = result[1].str() + "Textures";

	std::map<std::string, MtlData> mtls;
	MtlData mtl;

	std::ifstream file;
	file.exceptions(std::ifstream::badbit | std::ifstream::failbit | std::ifstream::eofbit);
	char junk;
	try {
		file.open(mtlPath);
		char line[128];
		while (file.getline(line, 128)) {
			if (std::regex_search(line, std::regex("newmtl"))) {
				std::regex reg("newmtl (.*)");
				std::smatch match;
				std::string strLine(line);
				std::regex_search(strLine, match, reg);
				//if (mtls.size() > 0) {
					mtls.insert(std::make_pair(mtl.name, mtl));
					mtl.name = match[1].str();
					mtl.diffuseMap = "";
				/*}
				else {
					mtl.name = match[1].str();
					mtl.diffuseMap = "";
				}*/
			}
			if (std::regex_search(line, std::regex("map_Kd"))) {
				std::regex reg("map_Kd (.*)");
				std::smatch match;
				std::string strLine(line);
				std::regex_search(strLine, match, reg);
				mtl.diffuseMap = folder + "/" + match[1].str();
			}
		}
	}
	catch (std::ifstream::failure e) {
		/*for (auto mtl : mtls) {
			std::cout << "Result:" << std::endl;
			std::cout << "Name: " << mtl.second.name << std::endl;
			std::cout << "Diffuse map: " << mtl.second.diffuseMap << std::endl;
			std::cout << "Texture: " << mtl.second.texture << std::endl;
			std::cout << std::endl;
		}*/
		if (!file.eof()) {
			std::cout << "MODEL_LOAD::FILE_ERROR::" << e.what() << std::endl;
		}
		else {
			mtls.insert(std::make_pair(mtl.name, mtl));
			file.close();
		}
	}
	return mtls;
}

void Model3D::LoadModel(const std::string& filePath, TexturesFactory* texturesFactory) {
	// TODO: Rewrite this with filesystem lib
	const std::regex folderPathReg(".*\/");
	std::smatch folderPathMatch;
	std::regex_search(filePath, folderPathMatch, folderPathReg);
	const std::string folderPath = folderPathMatch[0].str();

	std::vector<vgu::Vector3f> vertices;
	std::vector<vgu::Vector3f> normals;
	std::vector<vgu::Vector2f> textureVertices;
	std::vector<ModelFaceData> faces;
	std::map<std::string, MtlData> mtlData;
	std::string currentMtl = "";

	std::ifstream file;
	file.exceptions(std::ifstream::badbit | std::ifstream::failbit | std::ifstream::eofbit);
	char junk;
	// TODO: Fix here the problem with the file closing, it is always throws EOF exception, this should be fixed
	// TODO: Also this method if VERY big and really bad formatted, I should think about refactoring this
	try {
		file.open(filePath.c_str());
		char line[128];
		while (file.getline(line, 128)) {
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
						Texture* texture = texturesFactory->LoadTexture(mtlData[currentMtl].diffuseMap);
						if (mtlData[currentMtl].name == "Woman_Iris_Left") {
							std::cout << "Result: " << std::endl;
							std::cout << "Name: " << mtlData[currentMtl].name << std::endl;
							std::cout << "Texture : " << texture << std::endl;
							std::cout << std::endl;
						}
						faces.push_back({ verticesData[0], verticesData[(long long)i + 1], verticesData[(long long)i + 2], texture });
					}
				}
			}
			if (std::regex_search(line, std::regex("usemtl"))) {
				std::regex reg("usemtl (.*)");
				std::smatch match;
				std::string strLine(line);
				std::regex_search(strLine, match, reg);
				currentMtl = match[1].str();
			}
			if (std::regex_search(line, std::regex("mtllib"))) {
				// TODO: User filesystem lib insead if regex
				try {
					std::regex reg("mtllib (.*)");
					std::smatch res;
					std::string strLine(line);
					std::regex_search(strLine, res, reg);
					std::string mtllibFileName = res[1].str();

					mtlData = GetMtlData(folderPath + mtllibFileName);
				}
				catch (std::exception exp) {
					std::cout << exp.what() << std::endl;
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
				vgu::Vector2f tFirst = textureVertices[faces[i].data[0].texture];
				vgu::Vector2f tSecond = textureVertices[faces[i].data[1].texture];
				vgu::Vector2f tThird = textureVertices[faces[i].data[2].texture];
				if (faces[i].texture) {
					const double imgWidth = faces[i].texture->GetWidth();
					const double imgHeigth = faces[i].texture->GetHeight();
					tFirst.coord.x *= imgWidth;
					if (tFirst.coord.x < 0) tFirst.coord.x = 0;
					if (tFirst.coord.x >= imgWidth) tFirst.coord.x = imgWidth - 1;
					tFirst.coord.y *= imgHeigth;
					//tFirst.coord.y = imgHeigth * (1 - tFirst.coord.y);
					if (tFirst.coord.y < 0) tFirst.coord.y = 0;
					if (tFirst.coord.y >= imgHeigth) tFirst.coord.y = imgHeigth - 1;
					tSecond.coord.x *= imgWidth;
					if (tSecond.coord.x < 0) tSecond.coord.x = 0;
					if (tSecond.coord.x >= imgWidth) tSecond.coord.x = imgWidth - 1;
					tSecond.coord.y *= imgHeigth;
					//tSecond.coord.y = imgHeigth * (1 - tSecond.coord.y);
					if (tSecond.coord.y < 0) tSecond.coord.y = 0;
					if (tSecond.coord.y >= imgHeigth) tSecond.coord.y = imgHeigth - 1;
					tThird.coord.x *= imgWidth;
					if (tThird.coord.x < 0) tThird.coord.x = 0;
					if (tThird.coord.x >= imgWidth) tThird.coord.x = imgWidth - 1;
					tThird.coord.y *= imgHeigth;
					//tThird.coord.y = imgHeigth * (1 - tThird.coord.y);
					if (tThird.coord.y < 0) tThird.coord.y = 0;
					if (tThird.coord.y >= imgHeigth) tThird.coord.y = imgHeigth - 1;
				}
				m_polygons[i] = { first, second, third, tFirst, tSecond, tThird };
				m_polygons[i].CalculateNormal();
				m_polygons[i].texture = faces[i].texture;
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
