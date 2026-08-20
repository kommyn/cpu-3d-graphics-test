#include "Engine3D.h"

Engine3D::Engine3D() {
	m_zBuffer = NULL;
	m_modelsFact.SetTexturesFactory(&m_texturesFact);
	// TODO: Add ShowCursor function to EngineBase class
	ShowCursor(FALSE);
}

/*void Engine3D::InterpolatedHalfTriangle(const Pixel& a, const Pixel& b, float* coeffs) {
	float longestA = coeffs[0];
	float longestB = coeffs[1];
	float redA = coeffs[2];
	float redB = coeffs[3];
	float redC = coeffs[4];
	float greenA = coeffs[5];
	float greenB = coeffs[6];
	float greenC = coeffs[7];
	float blueA = coeffs[8];
	float blueB = coeffs[9];
	float blueC = coeffs[10];
	long segmentHeight = b.y - a.y;
	if (segmentHeight != 0) {
		// Coeffitients for the first line part of the triangle
		float segmentA = (float)(b.x - a.x) / segmentHeight;
		float segmentB = a.x - segmentA * a.y;

		for (long y = a.y; y <= b.y; ++y) {
			long xMin = static_cast<long>(segmentA * y + segmentB + 0.5);
			long xMax = static_cast<long>(longestA * y + longestB + 0.5);
			if (xMin > xMax) std::swap(xMin, xMax);
			for (long x = xMin; x <= xMax; ++x) {
				float fRed = redA * x + redB * y + redC;
				float fGreen = greenA * x + greenB * y + greenC;
				float fBlue = blueA * x + blueB * y + blueC;
				unsigned char red = 0;
				unsigned char green = 0;
				unsigned char blue = 0;
				if (fRed > 255) {
					red = 255;
				}
				else if (fRed < 0) {
					red = 0;
				}
				else red = static_cast<unsigned char>(fRed);
				if (fGreen > 255) {
					green = 255;
				}
				else if (fGreen < 0) {
					green = 0;
				}
				else green = static_cast<unsigned char>(fGreen);
				if (fBlue > 255) {
					blue = 255;
				}
				else if (fBlue < 0) {
					blue = 0;
				}
				else blue = static_cast<unsigned char>(fBlue);
				SetPixel({ x, y, { red, green, blue, 255 } });
			}
		}
	}
}*/

/*void Engine3D::InterpolatedTriangle(Pixel a, Pixel b, Pixel c) {
	if (a.y > b.y) {
		std::swap(a, b);
	}
	if (a.y > c.y) {
		std::swap(a, c);
	}
	if (b.y > c.y) {
		std::swap(b, c);
	}

	// Calculate color interpolation for three points
	// TODO: Clean code in here
	float delta = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
	float redA = ((b.color.r - a.color.r) * (c.y - a.y) - (c.color.r - a.color.r) * (b.y - a.y)) / delta;
	float redB = ((b.x - a.x) * (c.color.r - a.color.r) - (c.x - a.x) * (b.color.r - a.color.r)) / delta;
	float redC = (a.x * (b.y * c.color.r - c.y * b.color.r) - a.y * (b.x * c.color.r - c.x * b.color.r) + a.color.r * (b.x * c.y - c.x * b.y)) / delta;
	float greenA = ((b.color.g - a.color.g) * (c.y - a.y) - (c.color.g - a.color.g) * (b.y - a.y)) / delta;
	float greenB = ((b.x - a.x) * (c.color.g - a.color.g) - (c.x - a.x) * (b.color.g - a.color.g)) / delta;
	float greenC = (a.x * (b.y * c.color.g - c.y * b.color.g) - a.y * (b.x * c.color.g - c.x * b.color.g) + a.color.g * (b.x * c.y - c.x * b.y)) / delta;
	float blueA = ((b.color.b - a.color.b) * (c.y - a.y) - (c.color.b - a.color.b) * (b.y - a.y)) / delta;
	float blueB = ((b.x - a.x) * (c.color.b - a.color.b) - (c.x - a.x) * (b.color.b - a.color.b)) / delta;
	float blueC = (a.x * (b.y * c.color.b - c.y * b.color.b) - a.y * (b.x * c.color.b - c.x * b.color.b) + a.color.b * (b.x * c.y - c.x * b.y)) / delta;

	// Coeffitients for the longest part of the triangle
	float longestA = (float)(c.x - a.x) / (c.y - a.y);
	float longestB = a.x - longestA * a.y;

	float coeffs[11] = { longestA, longestB, redA, redB, redC, greenA, greenB, greenC, blueA, blueB, blueC };

	InterpolatedHalfTriangle(a, b, coeffs);
	InterpolatedHalfTriangle(b, c, coeffs);
}*/

void Engine3D::FillHalfTriangle(const Pixel& a, const Pixel& b, const double coeffs[3], const double coeefsD[3], Texture* texture, const double coeffsT[6]) {
	const double longestA = coeffs[0];
	const double longestB = coeffs[1];
	const double lightIntensivity = coeffs[2];
	const double zBuffA = coeefsD[0];
	const double zBuffB = coeefsD[1];
	const double zBuffC = coeefsD[2];

	const int segmentHeight = b.y - a.y;
	const int numOfPixels = m_pixelsHNum * m_pixelsWNum;

	if (segmentHeight != 0) {
		double segmentA = (double)(b.x - a.x) / segmentHeight;
		double segmentB = a.x - segmentA * a.y;

		for (int y = a.y; y <= b.y; ++y) {
			int xMin = static_cast<int>(segmentA * y + segmentB + 0.5);
			int xMax = static_cast<int>(longestA * y + longestB + 0.5);
			if (xMin > xMax) std::swap(xMin, xMax);
			for (int x = xMin; x <= xMax; ++x) {
				const double zBuffData = zBuffA * x + zBuffB * y + zBuffC;
				// TODO: Fix problem with types in here
				const unsigned long long zBuffPixelIndex = (long long)x + (static_cast<long long>(m_pixelsWNum + 0.5) - 1) * (long long)y;
				if (zBuffPixelIndex < numOfPixels && m_zBuffer[zBuffPixelIndex] > zBuffData) {
					unsigned char colorRed = (unsigned char)(lightIntensivity * 230);
					unsigned char colorGreen = (unsigned char)(lightIntensivity * 230);
					unsigned char colorBlue = (unsigned char)(lightIntensivity * 230);
					// TODO: Fix this, it is look terrible
					if (texture) {
						long long textureX = coeffsT[0] * x + coeffsT[1] * y + coeffsT[2];
						long long textureY = coeffsT[3] * x + coeffsT[4] * y + coeffsT[5];
						if (textureX >= texture->GetWidth()) textureX = static_cast<long long>(texture->GetWidth() + 0.5) - 1;
						if (textureX < 0) textureX = 0;
						if (textureY >= texture->GetHeight()) textureY = static_cast<long long>(texture->GetHeight() + 0.5) - 1;
						if (textureY < 0) textureY = 0;
						textureY = texture->GetHeight() - textureY - 1;
						const long long textureIndex = 4 * (textureX + textureY * texture->GetWidth());
						RGBAColor* color = (*texture)[textureIndex];
						if (color) {
							colorRed = color->b * lightIntensivity;
							colorGreen = color->g * lightIntensivity;
							colorBlue = color->r * lightIntensivity;
						}
					}
					SetPixel({ x, y, { colorRed, colorGreen, colorBlue, 255} });
					m_zBuffer[zBuffPixelIndex] = zBuffData;
				}
			}
		}
	}
}

vgu::Vector3f lightDir = vgu::normalize(vgu::Vector3f{1.0f, -1.0f, 0.0f});

// TODO: Rewrite this to be more optimised, I don't like how it's working right now;
void Engine3D::FillTriangle(const Polygon3P& polygon) {
	vgu::Vector4f A = polygon.m_first, B = polygon.m_second, C = polygon.m_third;
	int first = m_pixelsWNum - 1;
	int second = m_pixelsHNum - 1;
	A[0] = (A[0] * first + first) * 0.5;
	A[1] = (A[1] * second + second) * 0.5;
	B[0] = (B[0] * first + first) * 0.5;
	B[1] = (B[1] * second + second) * 0.5;
	C[0] =(C[0] * first + first) * 0.5;
	C[1] = (C[1] * second + second) * 0.5;

	Pixel a = { static_cast<int>(A[0] + 0.5), static_cast<int>(A[1] + 0.5) };
	Pixel b = { static_cast<int>(B[0] + 0.5), static_cast<int>(B[1] + 0.5) };
	Pixel c = { static_cast<int>(C[0] + 0.5), static_cast<int>(C[1] + 0.5) };

	vgu::Vector2f At = polygon.m_tFirst, Bt = polygon.m_tSecond, Ct = polygon.m_tThird;

	if (a.y > b.y) {
		std::swap(a, b);
		std::swap(A, B);
		std::swap(At, Bt);
	}
	if (a.y > c.y) {
		std::swap(a, c);
		std::swap(A, C);
		std::swap(At, Ct);
	}
	if (b.y > c.y) {
		std::swap(b, c);
		std::swap(B, C);
		std::swap(Bt, Ct);
	}

	//const double delta = (B[0] * C[1] - B[1] * C[0]) + (C[0] * A[1] - C[1] * A[0]) + (A[0] * B[1] - A[1] * B[0]);
	const double delta = (B[0] - A[0]) * (C[1] - A[1]) - (C[0] - A[0]) * (B[1] - A[1]);
	if (std::abs(delta) < 0.0000000001f) return;
	const double zBuffA = ((B[2] - A[2]) * (C[1] - A[1]) - (C[2] - A[2]) * (B[1] - A[1])) / delta;
	//const double zBuffA = (B[2] * C[1] - B[1] * C[2]) + (C[2] * A[1] - C[1] * A[2]) + (A[2] * B[1] - B[2] * A[1]);
	const double zBuffB = ((B[0] - A[0]) * (C[2] - A[2]) - (C[0] - A[0]) * (B[2] - A[2])) / delta;
	//const double zBuffB = (B[0] * C[2] - B[2] * C[0]) + (C[0] * A[2] - C[2] * A[0]) + (A[0] * B[2] - A[2] * B[0]);
	const double zBuffC = (A[0] * (B[1] * C[2] - C[1] * B[2]) - B[0] * (A[1] * C[2] - C[1] * A[2]) + C[0] * (A[1] * B[2] - B[1] * A[2])) / delta;

	// TODO: Do something with this calculation, it is really bad
	double textureCoeffs[6] = { 0, 0, 0, 0, 0, 0 };
	if (polygon.texture) {
		// Ax
		textureCoeffs[0] = ((Bt[0] - At[0]) * (C[1] - A[1]) - (Ct[0] - At[0]) * (B[1] - A[1])) / delta;
		// Bx
		textureCoeffs[1] = ((B[0] - A[0]) * (Ct[0] - At[0]) - (C[0] - A[0]) * (Bt[0] - At[0])) / delta;
		// Cx
		textureCoeffs[2] = (A[0] * (B[1] * Ct[0] - C[1] * Bt[0]) - B[0] * (A[1] * Ct[0] - C[1] * At[0]) + C[0] * (A[1] * Bt[0] - B[1] * At[0])) / delta;
		// Ay
		textureCoeffs[3] = ((Bt[1] - At[1]) * (C[1] - A[1]) - (Ct[1] - At[1]) * (B[1] - A[1])) / delta;
		// By
		textureCoeffs[4] = ((B[0] - A[0]) * (Ct[1] - At[1]) - (C[0] - A[0]) * (Bt[1] - At[1])) / delta;
		// Cy
		textureCoeffs[5] = (A[0] * (B[1] * Ct[1] - C[1] * Bt[1]) - B[0] * (A[1] * Ct[1] - C[1] * At[1]) + C[0] * (A[1] * Bt[1] - B[1] * At[1])) / delta;
	}

	const double lightIntensivity = (1 - vgu::dotProduct(lightDir, polygon.m_normal)) * 0.5f;

	const double longestA = (double)((double)c.x - (double)a.x) / ((double)c.y - (double)a.y);
	const double longestB = a.x - longestA * a.y;
	const double coeffs[3] = { longestA, longestB, lightIntensivity };
	const double coeefsD[3] = { zBuffA, zBuffB, zBuffC };
	
	FillHalfTriangle(a, b, coeffs, coeefsD, polygon.texture, textureCoeffs);
	FillHalfTriangle(b, c, coeffs, coeefsD, polygon.texture, textureCoeffs);
}

void Engine3D::DrawLine(Pixel a, Pixel b, const double coeffs[3]) {
	const double zBuffA = coeffs[0];
	const double zBuffB = coeffs[1];
	const double zBuffC = coeffs[2];

	bool steep = std::abs(b.y - a.y) > std::abs(b.x - a.x);
	if (steep) {
		std::swap(a.x, a.y);
		std::swap(b.x, b.y);
	}
	if (a.x > b.x) {
		std::swap(a.x, b.x);
		std::swap(a.y, b.y);
		std::swap(a.color.r, b.color.r);
		std::swap(a.color.g, b.color.g);
		std::swap(a.color.b, b.color.b);
		std::swap(a.color.a, b.color.a);
	}
	int dx = b.x - a.x;
	int dy = std::abs(b.y - a.y);
	int error = dx / 2;
	int yStep = (a.y < b.y) ? 1 : -1;
	int y = a.y;

	for (int x = a.x; x <= b.x; ++x) {
		int pointX = steep ? y : x;
		int pointY = steep ? x : y;

		if (x >= 0 && x < m_pixelsWNum && y >= 0 && y < m_pixelsHNum) {
			const double zBuffData = zBuffA * pointX + zBuffB * pointY + zBuffC;
			// TODO: Fix problem with types in here
			const unsigned long long zBuffPixelIndex = (long long)pointX + m_pixelsWNum * (long long)pointY;
			if (m_zBuffer[zBuffPixelIndex] >= zBuffData) {
				SetPixel({ pointX, pointY, { 200, 200, 200, 255 } });
				m_zBuffer[zBuffPixelIndex] = zBuffData;
			}
		}
		error -= dy;
		if (error < 0) {
			y += yStep;
			error += dx;
		}
	}
}

void Engine3D::DrawTriangle(const Polygon3P& polygon) {
	vgu::Vector4f A = polygon.m_first, B = polygon.m_second, C = polygon.m_third;
	A[0] = static_cast<int>(A[0] * m_pixelsWNum + m_pixelsWNum + 0.5) * 0.5;
	A[1] = static_cast<int>(A[1] * m_pixelsHNum + m_pixelsHNum + 0.5) * 0.5;
	B[0] = static_cast<int>(B[0] * m_pixelsWNum + m_pixelsWNum + 0.5) * 0.5;
	B[1] = static_cast<int>(B[1] * m_pixelsHNum + m_pixelsHNum + 0.5) * 0.5;
	C[0] = static_cast<int>(C[0] * m_pixelsWNum + m_pixelsWNum + 0.5) * 0.5;
	C[1] = static_cast<int>(C[1] * m_pixelsHNum + m_pixelsHNum + 0.5) * 0.5;
	
	Pixel a = { A[0], A[1] };
	Pixel b = { B[0], B[1] };
	Pixel c = { C[0], C[1] };
	if (a.y > b.y) {
		std::swap(a, b);
		std::swap(A, B);
	}
	if (a.y > c.y) {
		std::swap(a, c);
		std::swap(A, C);
	}
	if (b.y > c.y) {
		std::swap(b, c);
		std::swap(B, C);
	}

	const double delta = (B[0] - A[0]) * (C[1] - A[1]) - (C[0] - A[0]) * (B[1] - A[1]);
	if (std::abs(delta) < 0.0000001f) return;
	const double zBuffA = ((B[2] - A[2]) * (C[1] - A[1]) - (C[2] - A[2]) * (B[1] - A[1])) / delta;
	const double zBuffB = ((B[0] - A[0]) * (C[2] - A[2]) - (C[0] - A[0]) * (B[2] - A[2])) / delta;
	const double zBuffC = (A[0] * (B[1] * C[2] - C[1] * B[2]) - B[0] * (A[1] * C[2] - C[1] * A[2]) + C[0] * (A[1] * B[2] - B[1] * A[2])) / delta;

	const double coeffs[3] = { zBuffA, zBuffB, zBuffC };

	DrawLine(a, b, coeffs);
	DrawLine(b, c, coeffs);
	DrawLine(c, a, coeffs);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

bool isPointOutOfScreen(const vgu::Vector4f& point) {
	return (point.coord.x > 1 || point.coord.x < -1 || point.coord.y > 1 || point.coord.y < -1);
}


bool isPointBetween(const double& first, const double& second, const double& point) {
	return (point <= first&& point >= second) || (point <= second && point >= first);
}

vgu::Vector4f* calculateLinePlaneCrossing(
	const vgu::Vector4f& sectionPoint1,
	const vgu::Vector4f& sectionPoint2,
	double A, double B, double C, double D) {
	double A1 = sectionPoint2.coord.y - sectionPoint1.coord.y;
	double B1 = sectionPoint1.coord.x - sectionPoint2.coord.x;
	double C1 = 0;
	double D1 = sectionPoint2.coord.x * sectionPoint1.coord.y - sectionPoint1.coord.x * sectionPoint2.coord.y;

	const double A2 = 0;
	const double B2 = sectionPoint2.coord.z - sectionPoint1.coord.z;
	const double C2 = sectionPoint1.coord.y - sectionPoint2.coord.y;
	const double D2 = sectionPoint2.coord.y * sectionPoint1.coord.z - sectionPoint1.coord.y * sectionPoint2.coord.z;

	double det = vgu::det(vgu::Matrix3x3({
		A1, B1, C1,
		A2, B2, C2,
		A, B, C
	}));

	if (std::abs(det) < 0.000001) {
		A1 = sectionPoint2.coord.z - sectionPoint1.coord.z;
		B1 = 0;
		C1 = sectionPoint1.coord.x - sectionPoint2.coord.x;
		D1 = sectionPoint2.coord.x * sectionPoint1.coord.z - sectionPoint1.coord.x * sectionPoint2.coord.z;
		det = vgu::det(vgu::Matrix3x3({
			A1, B1, C1,
			A2, B2, C2,
			A, B, C
		}));
	}

	if (std::abs(det) < 0.000001) return nullptr;

	const double x = vgu::det(vgu::Matrix3x3({
		-D1, B1, C1,
		-D2, B2, C2,
		-D, B, C
	})) / det;
	const double y = vgu::det(vgu::Matrix3x3({
		A1, -D1, C1,
		A2, -D2, C2,
		A, -D, C
		})) / det;
	const double z = vgu::det(vgu::Matrix3x3({
		A1, B1, -D1,
		A2, B2, -D2,
		A, B, -D
		})) / det;

	if (
		isPointBetween(sectionPoint1.coord.x, sectionPoint2.coord.x, x) &&
		isPointBetween(sectionPoint1.coord.y, sectionPoint2.coord.y, y) &&
		isPointBetween(sectionPoint1.coord.z, sectionPoint2.coord.z, z)
		) {
		return new vgu::Vector4f({ x ,y, z, 1 });
	}

	return nullptr;
}

vgu::Vector4f* calculateCrossWithFourLines(const vgu::Vector4f& first, const vgu::Vector4f& second, const Side& side) {
	switch (side) {
	case Side::TOP:
		return calculateLinePlaneCrossing(first, second, 0, 1, 0, -1);
	case Side::BOTTOM:
		return calculateLinePlaneCrossing(first, second, 0, -1, 0, -1);
	case Side::LEFT:
		return calculateLinePlaneCrossing(first, second, -1, 0, 0, -1);
	case Side::RIGHT:
		return calculateLinePlaneCrossing(first, second, 1, 0, 0, -1);
	}
	return nullptr;
}


// TODO: Этот костыль позволяет определить о какой линии идёт речь, по-хорошему надо писать функцию для любой линии
boolean isPointOutOfLine(const vgu::Vector4f& point, const Side& side) {
	switch (side) {
	case Side::TOP:
		return point.coord.y > 1;
	case Side::LEFT:
		return point.coord.x < -1;
	case Side::RIGHT:
		return point.coord.x > 1;
	case Side::BOTTOM:
		return point.coord.y < -1;
	}
}

vgu::Vector2f calculateTexturePoint(const vgu::Vector4f& first, const vgu::Vector4f& second, const vgu::Vector2f& tFirst, const vgu::Vector2f& tSecond, const vgu::Vector4f& point) {
	const double bx = tFirst.coord.x;
	const double by = tFirst.coord.y;
	const double firstDivider = second.coord.x - first.coord.x;
	const double secondDivider = second.coord.y - first.coord.y;
	const double kx = (tSecond.coord.x - tFirst.coord.x) / firstDivider;
	const double ky = (tSecond.coord.y - tFirst.coord.y) / secondDivider;

	const double x = firstDivider == 0 ? tFirst.coord.x : (kx * (point.coord.x - first.coord.x) + bx);
	const double y = secondDivider == 0 ? tFirst.coord.y : (ky * (point.coord.y - first.coord.y) + by);

	return { x, y };
}

Polygon3P* clipPolyg(Polygon3P& polygon, int& size) {
	size = 0;
	Polygon3P* result;

	std::list<Polygon3P> polygonsQueue;
	polygonsQueue.push_back(polygon);

	for (size_t i = 0; i < 4; ++i) {
		std::list<Polygon3P>::iterator polygonIter;

		for (polygonIter = polygonsQueue.begin(); polygonIter != polygonsQueue.end(); ++polygonIter) {
			auto polyg = *polygonIter;
			// Find all points in/out of the screen
		    ////////////////////////////////////////////////////////////////////////////////
			vgu::Vector4f* pointsOut[3] = { nullptr, nullptr, nullptr };
			vgu::Vector2f* tPointsOut[3] = { nullptr, nullptr, nullptr };
			short pointsOutNum = 0;
			vgu::Vector4f* pointsIn[3] = { nullptr, nullptr, nullptr };
			vgu::Vector2f* tPointsIn[3] = { nullptr, nullptr, nullptr };
			short pointsInNum = 0;
			////////////////////////////////////////////////////////////////////////////////
			if (isPointOutOfLine(polyg.m_first, static_cast<Side>(i))) {
				tPointsOut[pointsOutNum] = &polyg.m_tFirst;
				pointsOut[pointsOutNum++] = &polyg.m_first;
			}
			else {
				tPointsIn[pointsInNum] = &polyg.m_tFirst;
				pointsIn[pointsInNum++] = &polyg.m_first;
			}
			////////////////////////////////////////////////////////////////////////////////
			if (isPointOutOfLine(polyg.m_second, static_cast<Side>(i))) {
				tPointsOut[pointsOutNum] = &polyg.m_tSecond;
				pointsOut[pointsOutNum++] = &polyg.m_second;
			}
			else {
				tPointsIn[pointsInNum] = &polyg.m_tSecond;
				pointsIn[pointsInNum++] = &polyg.m_second;
			}
			////////////////////////////////////////////////////////////////////////////////
			if (isPointOutOfLine(polyg.m_third, static_cast<Side>(i))) {
				tPointsOut[pointsOutNum] = &polyg.m_tThird;
				pointsOut[pointsOutNum++] = &polyg.m_third;
			}
			else {
				tPointsIn[pointsInNum] = &polyg.m_tThird;
				pointsIn[pointsInNum++] = &polyg.m_third;
			}
			////////////////////////////////////////////////////////////////////////////////

			if (pointsOutNum == 1) {
				vgu::Vector4f* result1 = calculateCrossWithFourLines(*pointsIn[0], *pointsOut[0], static_cast<Side>(i));
				vgu::Vector4f* result2 = calculateCrossWithFourLines(*pointsIn[1], *pointsOut[0], static_cast<Side>(i));

				if (result1 && result2) {
					vgu::Vector4f result1Form{ result1->coord.x, result1->coord.y, result1->coord.z, result1->coord.w };
					vgu::Vector4f result2Form{ result2->coord.x, result2->coord.y, result2->coord.z, result2->coord.w };

					delete result1;
					delete result2;

					vgu::Vector2f tRes1 = calculateTexturePoint(*pointsIn[0], *pointsOut[0], *tPointsIn[0], *tPointsOut[0], result1Form);
					vgu::Vector2f tRes2 = calculateTexturePoint(*pointsIn[1], *pointsOut[0], *tPointsIn[1], *tPointsOut[0], result2Form);

					Polygon3P newPolyg1{
						result1Form,
						*pointsIn[1],
						*pointsIn[0],
						tRes1,
						*tPointsIn[1],
						*tPointsIn[0],
						polyg.m_normal,
						polyg.texture
					};

					Polygon3P newPolyg2{
						result1Form,
						result2Form,
						*pointsIn[1],
						tRes1,
						tRes2,
						*tPointsIn[1],
						polyg.m_normal,
						polyg.texture
					};

					polygonsQueue.erase(polygonIter);
					polygonsQueue.push_back(newPolyg1);
					polygonsQueue.push_back(newPolyg2);
				}
			}

			if (pointsOutNum == 2) {
				vgu::Vector4f* result1 = calculateCrossWithFourLines(*pointsIn[0], *pointsOut[0], static_cast<Side>(i));
				vgu::Vector4f* result2 = calculateCrossWithFourLines(*pointsIn[0], *pointsOut[1], static_cast<Side>(i));

				if (result1 && result2) {
					vgu::Vector4f result1Form{ result1->coord.x, result1->coord.y, result1->coord.z, result1->coord.w };
					vgu::Vector4f result2Form{ result2->coord.x, result2->coord.y, result2->coord.z, result2->coord.w };

					vgu::Vector2f tRes1 = calculateTexturePoint(*pointsIn[0], *pointsOut[0], *tPointsIn[0], *tPointsOut[0], result1Form);
					vgu::Vector2f tRes2 = calculateTexturePoint(*pointsIn[0], *pointsOut[1], *tPointsIn[0], *tPointsOut[1], result2Form);

					delete result1;
					delete result2;

					Polygon3P newPolyg{
						result1Form,
						result2Form,
						*pointsIn[0],
						tRes1,
						tRes2,
						*tPointsIn[0],
						polyg.m_normal,
						polyg.texture
					};

					polygonsQueue.erase(polygonIter);
					polygonsQueue.push_back(newPolyg);
				}
			}

			if (pointsOutNum == 3) {
				polygonsQueue.erase(polygonIter);
			}
		}
	}

	size = polygonsQueue.size();
	result = new Polygon3P[size];
	size_t i = 0;
	for (auto& pol : polygonsQueue) {
		result[i] = pol;
		++i;
	}
	return result;
}

bool isPointOutOfLineNew(const vgu::Vector4f& point, const int& side) {
	switch (side) {
	case 0:
		return (point.coord.z < 0.1);
	case 1:
		return (point.coord.z > 100);
	}
	return false;
}

vgu::Vector4f* calculateCrossWithTwoLines(const vgu::Vector4f& first, const vgu::Vector4f& second, const int& side) {
	switch (side) {
	case 0:
		return calculateLinePlaneCrossing(first, second, 0, 0, 1, -0.1);
	case 1:
		return calculateLinePlaneCrossing(first, second, 0, 0, -1, -100);
	}
	return nullptr;
}

Polygon3P* clipPolygNew(Polygon3P& polygon, int& size) {
	size = 0;
	Polygon3P* result;

	std::list<Polygon3P> polygonsQueue;
	polygonsQueue.push_back(polygon);

	for (size_t i = 0; i < 4; ++i) {
		std::list<Polygon3P>::iterator polygonIter;

		for (polygonIter = polygonsQueue.begin(); polygonIter != polygonsQueue.end(); ++polygonIter) {
			auto polyg = *polygonIter;
			// Find all points in/out of the screen
			////////////////////////////////////////////////////////////////////////////////
			vgu::Vector4f* pointsOut[3] = { nullptr, nullptr, nullptr };
			vgu::Vector2f* tPointsOut[3] = { nullptr, nullptr, nullptr };
			short pointsOutNum = 0;
			vgu::Vector4f* pointsIn[3] = { nullptr, nullptr, nullptr };
			vgu::Vector2f* tPointsIn[3] = { nullptr, nullptr, nullptr };
			short pointsInNum = 0;
			////////////////////////////////////////////////////////////////////////////////
			if (isPointOutOfLineNew(polyg.m_first, i)) {
				tPointsOut[pointsOutNum] = &polyg.m_tFirst;
				pointsOut[pointsOutNum++] = &polyg.m_first;
			}
			else {
				tPointsIn[pointsInNum] = &polyg.m_tFirst;
				pointsIn[pointsInNum++] = &polyg.m_first;
			}
			////////////////////////////////////////////////////////////////////////////////
			if (isPointOutOfLineNew(polyg.m_second, i)) {
				tPointsOut[pointsOutNum] = &polyg.m_tSecond;
				pointsOut[pointsOutNum++] = &polyg.m_second;
			}
			else {
				tPointsIn[pointsInNum] = &polyg.m_tSecond;
				pointsIn[pointsInNum++] = &polyg.m_second;
			}
			////////////////////////////////////////////////////////////////////////////////
			if (isPointOutOfLineNew(polyg.m_third, i)) {
				tPointsOut[pointsOutNum] = &polyg.m_tThird;
				pointsOut[pointsOutNum++] = &polyg.m_third;
			}
			else {
				tPointsIn[pointsInNum] = &polyg.m_tThird;
				pointsIn[pointsInNum++] = &polyg.m_third;
			}
			////////////////////////////////////////////////////////////////////////////////

			if (pointsOutNum == 1) {
				vgu::Vector4f* result1 = calculateCrossWithTwoLines(*pointsIn[0], *pointsOut[0], i);
				vgu::Vector4f* result2 = calculateCrossWithTwoLines(*pointsIn[1], *pointsOut[0], i);

				if (result1 && result2) {
					vgu::Vector4f result1Form{ result1->coord.x, result1->coord.y, result1->coord.z, result1->coord.w };
					vgu::Vector4f result2Form{ result2->coord.x, result2->coord.y, result2->coord.z, result2->coord.w };

					delete result1;
					delete result2;

					vgu::Vector2f tRes1 = calculateTexturePoint(*pointsIn[0], *pointsOut[0], *tPointsIn[0], *tPointsOut[0], result1Form);
					vgu::Vector2f tRes2 = calculateTexturePoint(*pointsIn[1], *pointsOut[0], *tPointsIn[1], *tPointsOut[0], result2Form);

					Polygon3P newPolyg1{
						result1Form,
						*pointsIn[1],
						*pointsIn[0],
						tRes1,
						*tPointsIn[1],
						*tPointsIn[0],
						polyg.m_normal,
						polyg.texture
					};

					Polygon3P newPolyg2{
						result1Form,
						result2Form,
						*pointsIn[1],
						tRes1,
						tRes2,
						*tPointsIn[1],
						polyg.m_normal,
						polyg.texture
					};

					polygonsQueue.erase(polygonIter);
					polygonsQueue.push_back(newPolyg1);
					polygonsQueue.push_back(newPolyg2);
				}
			}

			if (pointsOutNum == 2) {
				vgu::Vector4f* result1 = calculateCrossWithTwoLines(*pointsIn[0], *pointsOut[0], i);
				vgu::Vector4f* result2 = calculateCrossWithTwoLines(*pointsIn[0], *pointsOut[1], i);

				if (result1 && result2) {
					vgu::Vector4f result1Form{ result1->coord.x, result1->coord.y, result1->coord.z, result1->coord.w };
					vgu::Vector4f result2Form{ result2->coord.x, result2->coord.y, result2->coord.z, result2->coord.w };

					vgu::Vector2f tRes1 = calculateTexturePoint(*pointsIn[0], *pointsOut[0], *tPointsIn[0], *tPointsOut[0], result1Form);
					vgu::Vector2f tRes2 = calculateTexturePoint(*pointsIn[0], *pointsOut[1], *tPointsIn[0], *tPointsOut[1], result2Form);

					delete result1;
					delete result2;

					Polygon3P newPolyg{
						result1Form,
						result2Form,
						*pointsIn[0],
						tRes1,
						tRes2,
						*tPointsIn[0],
						polyg.m_normal,
						polyg.texture
					};

					polygonsQueue.erase(polygonIter);
					polygonsQueue.push_back(newPolyg);
				}
			}

			if (pointsOutNum == 3) {
				polygonsQueue.erase(polygonIter);
			}
		}

	}

	size = polygonsQueue.size();
	result = new Polygon3P[size];
	size_t i = 0;
	for (auto& pol : polygonsQueue) {
		result[i] = pol;
		++i;
	}
	return result;
}
