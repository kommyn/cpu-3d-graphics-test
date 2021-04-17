#pragma once

#include "GraphUtils.h"
#include "Texture.h"

class Polygon3P
{ 
public:
	// Public variables
	vgu::Vector4f m_first, m_second, m_third;
	vgu::Vector3f m_normal;
	Texture* texture;
	bool ignorePolygon;

	// Contructors and destructors
	~Polygon3P() = default;

	// Other methods
	void CalculateNormal();
	void PerspectiveDivision();

	// Operators
	friend Polygon3P operator*(const vgu::Matrix4x4& matrix, Polygon3P polygon);
};

