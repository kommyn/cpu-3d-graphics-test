#pragma once

#include <vector>
#include <sstream>
#include <fstream>
#include <regex>
#include <set>
#include <filesystem>

#include "TexturesFactory.h"
#include "Polygon3P.h"

struct VertexData {
	int vertice, texture, normal;
};

// TODO: Move this to another file
struct ModelFaceData {
	VertexData data[3];
	Texture* texture;

	const VertexData& operator[](const int& index) const {
		return data[index];
	}
};

struct MtlData {
	std::string name;
	std::string diffuseMap;
	Texture* texture;
};

// TODO: Create ModelsFactory like textures factory I already use
class Model3D {
private:
	// TODO: Add here link to the textures factory that should be awailiable with the aggregation
	size_t m_polygonsSize;
	Polygon3P* m_polygons;
public:
	Model3D();
	~Model3D();

	// TODO: Refactor this method:
	//       1. Process all data with caution. Test for regex results and file error catching
	//       2. User filesystem lib for resolving paths
	void LoadModel(const std::string& filePath, TexturesFactory* texturesFactory);
	// TODO: Refactor this method:
	//       1. Process all data with caution. Test for regex results and file error catching
	//       2. User filesystem lib for resolving paths
	std::map<std::string, MtlData> GetMtlData(const std::string& mtlPath);
	// TODO: I don't like this method because it is very heavy so I believe I must find another way of polygons "in place" calculation
	Polygon3P* GetPolygons();
	size_t GetPolygonsSize() const;
};