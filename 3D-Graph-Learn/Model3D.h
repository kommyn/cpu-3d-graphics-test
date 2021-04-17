#pragma once

#include <vector>
#include <sstream>
#include <fstream>
#include <regex>

#include "TexturesFactory.h"
#include "Polygon3P.h"

struct VertexData {
	int vertice, texture, normal;
};

// TODO: Move this to another file
struct ModelFaceData {
	VertexData data[3];

	const VertexData& operator[](const int& index) const {
		return data[index];
	}
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

	void LoadModel(const std::string& filePath);
	// TODO: I don't like this method because it is very heavy so I believe I must find another way of polygons "in place" calculation
	Polygon3P* GetPolygons();
	size_t GetPolygonsSize() const;
};