#pragma once

#include "GraphUtils.h"
#include "Texture.h"

class Polygon3P
{ 
public:
	// Public variables
	// Vertices
	vgu::Vector4f m_first, m_second, m_third;
	// Texture s
	vgu::Vector2f m_tFirst, m_tSecond, m_tThird;
	// Normal to the poly
	vgu::Vector3f m_normal;
	Texture* texture;
	bool m_ignorePolygon;
	bool m_clipPolygon;

	// Contructors and destructors
	~Polygon3P() = default;

	// Other methods
	void CalculateNormal();
	void PerspectiveDivision();

	// Operators
	friend Polygon3P operator*(const vgu::Matrix4x4& matrix, Polygon3P polygon);

	friend bool operator==(const Polygon3P& lhs, const Polygon3P& rhs) {
		/*std::cout << "lhs: " << &lhs << std::endl;
		std::cout << "rhs: " << &rhs << std::endl;*/
		return &lhs == &rhs;
	}

	Polygon3P& operator=(const Polygon3P& rhs) {
		m_first = rhs.m_first;
		m_second = rhs.m_second;
		m_third = rhs.m_third;
		m_tFirst = rhs.m_tFirst;
		m_tSecond = rhs.m_tSecond;
		m_tThird = rhs.m_tThird;
		m_normal = rhs.m_normal;
		texture = rhs.texture;
		return *this;
	}

	void display() {
		std::cout << "Polygon3P first: ";
		m_first.display();
		std::cout << "Polygon3P second: ";
		m_second.display();
		std::cout << "Polygon3P third: ";
		m_third.display();
	}
};

