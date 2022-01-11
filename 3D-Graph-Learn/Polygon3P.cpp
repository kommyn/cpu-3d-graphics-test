#include "Polygon3P.h"

void Polygon3P::CalculateNormal() {
	const vgu::Vector3f fV = vgu::vecToEuclid(m_second - m_first);
	const vgu::Vector3f sV = vgu::vecToEuclid(m_third - m_first);
	m_normal = vgu::crossProduct(fV, sV);
	m_clipPolygon = false;
}

void Polygon3P::PerspectiveDivision() {
	m_ignorePolygon = false;
	vgu::Vector4f points[3] = { m_first, m_second, m_third };
	for (int i = 0; i < 3; ++i) {
		if (points[i].coord.w != 0) {
			points[i] /= points[i].coord.w;
		}
		else {
			m_ignorePolygon = true;
			return;
		}
	}
	m_first = points[0];
	m_second = points[1];
	m_third = points[2];
}

Polygon3P operator*(const vgu::Matrix4x4& matrix, Polygon3P polygon) {
	polygon.m_first = matrix * polygon.m_first;
	polygon.m_second = matrix * polygon.m_second;
	polygon.m_third = matrix * polygon.m_third;
	return polygon;
}