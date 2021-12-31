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
			int xMin = static_cast<int>(segmentA * y + segmentB + 0.5f);
			int xMax = static_cast<int>(longestA * y + longestB + 0.5f);
			if (xMin > xMax) std::swap(xMin, xMax);
			for (int x = xMin; x <= xMax; ++x) {
				const double zBuffData = zBuffA * x + zBuffB * y + zBuffC;
				// TODO: Fix problem with types in here
				const unsigned long long zBuffPixelIndex = (long long)x + (m_pixelsWNum - 1) * (long long)y;
				if (zBuffPixelIndex < numOfPixels && m_zBuffer[zBuffPixelIndex] > zBuffData) {
					unsigned char colorRed = (unsigned char)(lightIntensivity * 230);
					unsigned char colorGreen = (unsigned char)(lightIntensivity * 230);
					unsigned char colorBlue = (unsigned char)(lightIntensivity * 230);
					// TODO: Fix this, it is look terrible
					if (texture) {
						long long textureX = coeffsT[0] * x + coeffsT[1] * y + coeffsT[2];
						long long textureY = coeffsT[3] * x + coeffsT[4] * y + coeffsT[5];
						if (textureX >= texture->GetWidth()) textureX = texture->GetWidth() - 1;
						if (textureX < 0) textureX = 0;
						if (textureY >= texture->GetHeight()) textureY = texture->GetHeight() - 1;
						if (textureY < 0) textureY = 0;
						textureY = texture->GetHeight() - textureY - 1;
						const long long textureIndex = 4 * (textureX + textureY * texture->GetWidth());
						// RGBAColor* color = (*texture)[textureIndex];
						RGBAColor* color = (*texture)[textureIndex];
						if (color) {
							colorRed = color->b * lightIntensivity;
							colorGreen = color->g * lightIntensivity;
							colorBlue = color->r * lightIntensivity;
						}
					}
					SetPixel({ x, y, { 255, 255, 255, 255} });
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

	Pixel a = { static_cast<int>(A[0]), static_cast<int>(A[1]) };
	Pixel b = { static_cast<int>(B[0]), static_cast<int>(B[1]) };
	Pixel c = { static_cast<int>(C[0]), static_cast<int>(C[1]) };

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

	const double delta = (B[0] - A[0]) * (C[1] - A[1]) - (C[0] - A[0]) * (B[1] - A[1]);
	if (std::abs(delta) < 0.001f) return;
	const double zBuffA = ((B[2] - A[2]) * (C[1] - A[1]) - (C[2] - A[2]) * (B[1] - A[1])) / delta;
	const double zBuffB = ((B[0] - A[0]) * (C[2] - A[2]) - (C[0] - A[0]) * (B[2] - A[2])) / delta;
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

	const double longestA = (double)(c.x - a.x) / (c.y - a.y);
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
	A[0] = static_cast<int>(A[0] * m_pixelsWNum + m_pixelsWNum) * 0.5;
	A[1] = static_cast<int>(A[1] * m_pixelsHNum + m_pixelsHNum) * 0.5;
	B[0] = static_cast<int>(B[0] * m_pixelsWNum + m_pixelsWNum) * 0.5;
	B[1] = static_cast<int>(B[1] * m_pixelsHNum + m_pixelsHNum) * 0.5;
	C[0] = static_cast<int>(C[0] * m_pixelsWNum + m_pixelsWNum) * 0.5;
	C[1] = static_cast<int>(C[1] * m_pixelsHNum + m_pixelsHNum) * 0.5;
	
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
	if (std::abs(delta) < 0.00001f) return;
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

vgu::Vector2f* calculateLineSectionCrossing(
	const vgu::Vector2f& sectionPoint1,
	const vgu::Vector2f& sectionPoint2,
	const vgu::Vector2f& linePoint1,
	const vgu::Vector2f& linePoint2) {
	const double sectionA = sectionPoint2.coord.y - sectionPoint1.coord.y;
	const double sectionB = sectionPoint1.coord.x - sectionPoint2.coord.x;
	const double sectionC = - sectionPoint2.coord.x * sectionPoint1.coord.y + sectionPoint1.coord.x * sectionPoint2.coord.y;
	const double lineA = linePoint2.coord.y - linePoint1.coord.y;
	const double lineB = linePoint1.coord.x - linePoint2.coord.x;
	const double lineC = - linePoint2.coord.x * linePoint1.coord.y + linePoint1.coord.x * linePoint2.coord.y;

	double det = vgu::det(vgu::Matrix2x2({
		sectionA, sectionB,
		lineA, lineB
	}));
	std::cout << "Det: " << det << std::endl;
	if (std::abs(det) < 0.000001) return new vgu::Vector2f({ sectionPoint1.coord.x, sectionPoint1.coord.y });

	double x = vgu::det(vgu::Matrix2x2({
		sectionC, sectionB,
		lineC, lineB
		})) / det;
	double y = vgu::det(vgu::Matrix2x2({
		sectionA, sectionC,
		lineA, lineC
		})) / det;

	std::cout << "X: " << x << "\nY: " << y << std::endl;
	std::cout << "Points: " << std::endl;
	sectionPoint1.display();
	sectionPoint2.display();
	std::cout << "Check 1: ";
	bool result = sectionPoint1[0] > sectionPoint2[0] ? (x < sectionPoint1[0] && x > sectionPoint2[0]) : (x > sectionPoint1[0] && x < sectionPoint2[0]);
	std::cout << result << std::endl;

	if (
		(x < sectionPoint1.coord.x && x > sectionPoint2.coord.x) || (x < sectionPoint2.coord.x && x > sectionPoint1.coord.x) &&
		(y < sectionPoint1.coord.y && y > sectionPoint2.coord.y) || (y < sectionPoint2.coord.y && y > sectionPoint1.coord.y)
		) {
		return new vgu::Vector2f({ x ,y });
	}
	else {
		return nullptr;
	}
}

template <unsigned int N>
vgu::Vector2f* calculateCrossWithFourLines(const vgu::Vector<N>& first, const vgu::Vector<N>& second, Side side) {
	if (N < 2) return nullptr;
	vgu::Vector2f planeFirst = { first.m_coords[0], first.m_coords[1] };
	vgu::Vector2f planeSecond = { second.m_coords[0], second.m_coords[1] };
	switch (side) {
	case Side::TOP:
		return calculateLineSectionCrossing(
		planeFirst,
		planeSecond,
		{ -1, 1 },
		{ 1, 1 }
				 );
	case Side::BOTTOM:
		return calculateLineSectionCrossing(
			planeFirst,
			planeSecond,
			{ -1, -1 },
			{ 1, -1 }
		);
	case Side::LEFT:
		return calculateLineSectionCrossing(
			planeFirst,
			planeSecond,
			{ -1, -1 },
			{ -1, 1 }
		);
	case Side::RIGHT:
		return calculateLineSectionCrossing(
			planeFirst,
			planeSecond,
			{ 1, -1 },
			{ 1, 1 }
		);
	}
	return nullptr;
}

// TODO: Этот костыль позволяет определить о какой линии идёт речь, по-хорошему надо писать функцию для любой линии
boolean isPointOutOfLine(const vgu::Vector4f& point, size_t i) {
	switch (i) {
	case 0:
		return point.coord.y > 1;
	case 1:
		return point.coord.x < -1;
	case 2:
		return point.coord.x > 1;
	case 3:
		return point.coord.y < -1;
	}
}

Polygon3P* clipPolyg(Polygon3P& polygon, int& size, Side side) {
	if (static_cast<int>(side) > 3) return &polygon;

	size = 0;
	Polygon3P* result;

	std::vector<std::reference_wrapper<Polygon3P>> polygonsQueue;
	polygonsQueue.push_back(polygon);

	for (size_t i = 0; i < 4; ++i) {
		vgu::Vector2f lineFirst, lineSecond;
		switch (i) {
		case 0:
			lineFirst = { -1, 1 };
			lineSecond = { 1, 1 };
			break;
		case 1:
			lineFirst = { -1, 1 };
			lineSecond = { -1, -1 };
			break;
		case 2:
			lineFirst = { 1, 1 };
			lineSecond = { 1, -1 };
			break;
		case 3:
			lineFirst = { -1, -1 };
			lineSecond = { 1, -1 };
			break;
		}
		std::cout << "i: " << i << std::endl;

		int j = 0;

		for (auto polyg : polygonsQueue) {
			std::cout << "\n\n";
			std::cout << "Iteratin number: " << j << std::endl;
			// Find all points in/out of the screen
		    ////////////////////////////////////////////////////////////////////////////////
			vgu::Vector4f* pointsOut[3] = { nullptr, nullptr, nullptr };
			short pointsOutNum = 0;
			vgu::Vector4f* pointsIn[3] = { nullptr, nullptr, nullptr };
			short pointsInNum = 0;
			////////////////////////////////////////////////////////////////////////////////
			if (isPointOutOfLine(polygon.m_first, i)) pointsOut[pointsOutNum++] = &polygon.m_first;
			else pointsIn[pointsInNum++] = &polygon.m_first;
			////////////////////////////////////////////////////////////////////////////////
			if (isPointOutOfLine(polygon.m_second, i)) pointsOut[pointsOutNum++] = &polygon.m_second;
			else pointsIn[pointsInNum++] = &polygon.m_second;
			////////////////////////////////////////////////////////////////////////////////
			if (isPointOutOfLine(polygon.m_third, i)) pointsOut[pointsOutNum++] = &polygon.m_third;
			else pointsIn[pointsInNum++] = &polygon.m_third;
			////////////////////////////////////////////////////////////////////////////////

			std::cout << "i: " << i << std::endl;
			std::cout << "pointsOutNum: " << pointsOutNum << std::endl;
			std::cout << "polygonsQueue.empty(): " << polygonsQueue.empty() << std::endl;

			if (pointsOutNum == 1) {
				std::cout << "One point out" << i << std::endl;
				vgu::Vector2f* result1 = calculateCrossWithFourLines<4>(*pointsIn[0], *pointsOut[0], static_cast<Side>(i));
				vgu::Vector4f* result1Form = nullptr;
				std::cout << "Result 1 address: " << result1 << std::endl;
				if (result1) {
					result1Form = new vgu::Vector4f{ result1->coord.x, result1->coord.y, pointsIn[0]->coord.z, pointsIn[0]->coord.w };
					result1->display();
					delete result1;
				}

				std::cout << "\n";
				vgu::Vector2f* result2 = calculateCrossWithFourLines<4>(*pointsIn[1], *pointsOut[0], static_cast<Side>(i));
				vgu::Vector4f* result2Form = nullptr;
				std::cout << "Result 2 address: " << result2 << std::endl;
				if (result2) {
					result2Form = new vgu::Vector4f{ result2->coord.x, result2->coord.y, pointsIn[1]->coord.z, pointsIn[1]->coord.w };
					result2Form->display();
					std::cout << "Rar" << "\n\n";
					result2->display();
					delete result2;
				}
				std::cout << "\n\n";

				if (!(result1Form && result2Form)) continue;

				Polygon3P* newPolyg1 = new Polygon3P{
					*result1Form,
					*pointsIn[1],
					*pointsIn[0],
					polyg.get().m_tFirst,
					polyg.get().m_tSecond,
					polyg.get().m_tThird,
					polyg.get().m_normal
				};

				Polygon3P* newPolyg2 = new Polygon3P{
					*result1Form,
					*result2Form,
					*pointsIn[1],
					polyg.get().m_tFirst,
					polyg.get().m_tSecond,
					polyg.get().m_tThird,
					polyg.get().m_normal
				};

				polygonsQueue.pop_back();
				polygonsQueue.push_back(*newPolyg1);
				polygonsQueue.push_back(*newPolyg2);
			}
		}

	}

	size = polygonsQueue.size();
	result = new Polygon3P[size];
	size_t i = 0;
	for (auto pol : polygonsQueue) {
		result[i] = pol.get();
		++i;
	}
	return result;

	// Find all points in/out of the screen
	////////////////////////////////////////////////////////////////////////////////
	vgu::Vector4f* pointsOut[3] = { nullptr, nullptr, nullptr };
	short pointsOutNum = 0;
	vgu::Vector4f* pointsIn[3] = { nullptr, nullptr, nullptr };
	short pointsInNum = 0;
	////////////////////////////////////////////////////////////////////////////////
	if (isPointOutOfScreen(polygon.m_first)) pointsOut[pointsOutNum++] = &polygon.m_first;
	else pointsIn[pointsInNum++] = &polygon.m_first;
	////////////////////////////////////////////////////////////////////////////////
	if (isPointOutOfScreen(polygon.m_second)) pointsOut[pointsOutNum++] = &polygon.m_second;
	else pointsIn[pointsInNum++] = &polygon.m_second;
	////////////////////////////////////////////////////////////////////////////////
	if (isPointOutOfScreen(polygon.m_third)) pointsOut[pointsOutNum++] = &polygon.m_third;
	else pointsIn[pointsInNum++] = &polygon.m_third;
	////////////////////////////////////////////////////////////////////////////////

	//std::cout << "pointsOutNum: " << pointsOutNum << std::endl;
	if (pointsOutNum == 1) {
		/*std::cout << "One point out" << std::endl;
		vgu::Vector2f* result1 = calculateCrossWithFourLines<4>(*pointsIn[0], *pointsOut[0], Side::BOTTOM);
		std::cout << "Result 1 address: " << result1 << std::endl;
		if (result1) {
			result1->display();
			delete result1;
		}


		std::cout << "\n";
		vgu::Vector2f* result2 = calculateCrossWithFourLines<4>(*pointsIn[1], *pointsOut[0], Side::BOTTOM);
		std::cout << "Result 2 address: " << result2 << std::endl;
		if (result2) {
			result2->display();
			delete result2;
		}
		std::cout << "\n\n";*/

		/*Polygon3P newPolyg1 = { result1, pointsIn[1], pointsIn[0] };
		Polygon3P newPolyg2 = { pointsIn[1], result1, result2 };
		result = new Polygon3P[2];
		result[0] = newPolyg1;
		result[1] = newPolyg2;
		size = 2;
		return result;*/
	}

	// If all polygon located in screen just return itself
	size = 1;
	result = new Polygon3P[1];
	result[0] = polygon;
	return result;
}

Polygon3P* clipPolygon(Polygon3P& polygon, int& size) {
	size = 0;
	Polygon3P* result;
	////////////////////////////////////////////////////////////////////////////////
	vgu::Vector4f* pointsOut[3] = { nullptr, nullptr, nullptr };
	short pointsOutNum = 0;
	vgu::Vector4f* pointsIn[3] = { nullptr, nullptr, nullptr };
	short pointsInNum = 0;
	////////////////////////////////////////////////////////////////////////////////
	if (isPointOutOfScreen(polygon.m_first)) pointsOut[pointsOutNum++] = &polygon.m_first;
	else pointsIn[pointsInNum++] = &polygon.m_first;
	////////////////////////////////////////////////////////////////////////////////
	if (isPointOutOfScreen(polygon.m_second)) pointsOut[pointsOutNum++] = &polygon.m_second;
	else pointsIn[pointsInNum++] = &polygon.m_second;
	////////////////////////////////////////////////////////////////////////////////
	if (isPointOutOfScreen(polygon.m_third)) pointsOut[pointsOutNum++] = &polygon.m_third;
	else pointsIn[pointsInNum++] = &polygon.m_third;
	////////////////////////////////////////////////////////////////////////////////
	// std::cout << "pointsOutNum: " << pointsOutNum << std::endl;
	if (pointsOutNum == 1) {
		//result = new Polygon3P[2];
		// y == 1
		double xCoords[2] = { 0, 0 };
		bool isYSecond = false;
		double yCoords[2] = { 0, 0 };
		bool isXSecond = false;
		for (size_t i = 0; i < 2; ++i) {
			xCoords[i] = (pointsOut[0]->coord.x - pointsIn[i]->coord.x)
				* ((1 - pointsIn[i]->coord.y)
					/(pointsOut[0]->coord.y - pointsIn[i]->coord.y))
				+ pointsIn[i]->coord.x;
			if ((pointsOut[0]->coord.x > pointsIn[i]->coord.x && (xCoords[i] > pointsOut[0]->coord.x || xCoords[i] < pointsIn[i]->coord.x))
				|| (pointsOut[0]->coord.x < pointsIn[i]->coord.x && (xCoords[i] < pointsOut[0]->coord.x || xCoords[i] > pointsIn[i]->coord.x))) {
				break;
			}
		}
		/*std::cout << "Point out: " << pointsOut[0]->coord.x << "; " << pointsOut[0]->coord.y << std::endl;
		std::cout << "Point in: " << pointsIn[0]->coord.x << "; " << pointsIn[0]->coord.y << std::endl;
		std::cout << "xCoords 1 : " << xCoords[0] << "; " << xCoords[1] << std::endl;*/
		// y = -1
		if (xCoords[1] == 0) {
			isYSecond = true;
			for (size_t i = 0; i < 2; ++i) {
				xCoords[i] = (pointsOut[0]->coord.x - pointsIn[i]->coord.x)
					* ((-1 - pointsIn[i]->coord.y)
						/ (pointsOut[0]->coord.y - pointsIn[i]->coord.y))
					+ pointsIn[i]->coord.x;
				if ((pointsOut[0]->coord.x > pointsIn[i]->coord.x && (xCoords[i] > pointsOut[0]->coord.x || xCoords[i] < pointsIn[i]->coord.x))
					|| (pointsOut[0]->coord.x < pointsIn[i]->coord.x && (xCoords[i] < pointsOut[0]->coord.x || xCoords[i] > pointsIn[i]->coord.x))) {
					break;
				}
			}
		}
		// std::cout << "xCoords 2: " << xCoords[0] << "; " << xCoords[1] << std::endl;
		// x == 1
		if (xCoords[1] == 0) {
			for (size_t i = 0; i < 2; ++i) {
				yCoords[i] = (pointsOut[0]->coord.y - pointsIn[i]->coord.y)
					* ((1 - pointsIn[i]->coord.x)
						/ (pointsOut[0]->coord.x - pointsIn[i]->coord.x))
					+ pointsIn[i]->coord.y;
				if ((pointsOut[0]->coord.y > pointsIn[i]->coord.y && (yCoords[i] > pointsOut[0]->coord.y || yCoords[i] < pointsIn[i]->coord.y))
					|| (pointsOut[0]->coord.y < pointsIn[i]->coord.y && (yCoords[i] < pointsOut[0]->coord.y || yCoords[i] > pointsIn[i]->coord.y))) {
					break;
				}
			}
		}
		//std::cout << "yCoords 1: " << yCoords[0] << "; " << yCoords[1] << std::endl;
		// x == -1
		if (xCoords[1] == 0 && yCoords[1] == 0) {
			isXSecond = true;
			for (size_t i = 0; i < 2; ++i) {
				yCoords[i] = (pointsOut[0]->coord.y - pointsIn[i]->coord.y)
					* ((-1 - pointsIn[i]->coord.x)
						/ (pointsOut[0]->coord.x - pointsIn[i]->coord.x))
					+ pointsIn[i]->coord.y;
			}
		}
		//std::cout << "yCoords 1: " << yCoords[0] << "; " << yCoords[1] << std::endl;
		////////////////////////////////////////////////////////////////////////////

		vgu::Matrix2x2 deltaMatrix = {
			pointsIn[0]->coord.x, pointsIn[0]->coord.y,
			pointsOut[0]->coord.x, pointsOut[0]->coord.y
		};
		vgu::Matrix2x2 matrixA = {
			pointsIn[0]->coord.z, pointsIn[0]->coord.y,
			pointsIn[0]->coord.z, pointsOut[0]->coord.y,
		};
		vgu::Matrix2x2 matrixB = {
			pointsIn[0]->coord.x, pointsIn[0]->coord.z,
			pointsOut[0]->coord.x, pointsOut[0]->coord.z
		};
		double delta = vgu::det(deltaMatrix);
		double A = vgu::det(matrixA) / delta;
		double B = vgu::det(matrixB) / delta;

		////////////////////////////////////////////////////////////////////////////
		vgu::Vector4f firstPoint, secondPoint;
		if (xCoords[1] != 0) {
			double Y = isYSecond ? -1 : 1;
			firstPoint = { xCoords[0], Y, A * xCoords[0] + B * Y, pointsIn[0]->coord.w };
			secondPoint = { xCoords[1], Y, A * xCoords[1] + B * Y, pointsIn[1]->coord.w };
			/*firstPoint.coord.x = xCoords[0];
			firstPoint.coord.y = Y;
			firstPoint.coord.z = pointsIn[0]->coord.z;
			firstPoint.coord.w = pointsIn[0]->coord.w;
			secondPoint.coord.x = xCoords[1];
			secondPoint.coord.y = Y;
			secondPoint.coord.z = pointsIn[1]->coord.z;
			secondPoint.coord.w = pointsIn[1]->coord.w;*/
		}
		else {
			double X = isXSecond ? -1 : 1;
			firstPoint = { X, yCoords[0], A * X + B * yCoords[0], pointsIn[0]->coord.w };
			secondPoint = { X, yCoords[1], A * X + B * yCoords[1], pointsIn[1]->coord.w };
			/*firstPoint.coord.x = X;
			firstPoint.coord.y = yCoords[0];
			firstPoint.coord.z = pointsIn[0]->coord.z;
			firstPoint.coord.w = pointsIn[0]->coord.w;
			secondPoint.coord.x = xCoords[1];
			secondPoint.coord.y = yCoords[1];
			secondPoint.coord.z = pointsIn[1]->coord.z;
			secondPoint.coord.w = pointsIn[1]->coord.w;*/
		}
		size = 1;
	result = new Polygon3P[1];
		result[0] = polygon;
		return result;
		Polygon3P** polygons = new Polygon3P*[2];
		int sizes[2] = { 0, 0 };
		for (size_t i = 0; i < 2; ++i) {
			// TODO: Add to Polygon3P class method clone
			Polygon3P newPolygon;
			newPolygon.m_first = i == 0 ? *pointsIn[i] : *pointsIn[i];
			newPolygon.m_second = i == 0 ? *pointsIn[i + 1] : firstPoint;
			newPolygon.m_third = secondPoint;
			newPolygon.m_tFirst = polygon.m_tFirst;
			newPolygon.m_tSecond = polygon.m_tSecond;
			newPolygon.m_tThird = polygon.m_tThird;
			newPolygon.texture = polygon.texture;
			newPolygon.m_normal = vgu::crossProduct(
				vgu::vecToEuclid(
					newPolygon.m_second - newPolygon.m_first),
				vgu::vecToEuclid(newPolygon.m_third - newPolygon.m_first)
			);
			int newSize = 0;
			polygons[i] = clipPolygon(newPolygon, newSize);
			sizes[i] = newSize;
			size += newSize;
		}
		result = new Polygon3P[size];
		for (size_t i = 0; i < 2; ++i) {
			/*std::cout << "Size " << i << ": " << sizes[i] << std::endl;
			std::cout << std::endl;*/
			for (size_t j = 0; j < sizes[i]; ++j) {
				/*std::cout << "Polygon " << i << ", " << j << ": " << std::endl;
				polygons[i][j].display();*/
				result[j + i * sizes[i]] = polygons[i][j];
			}
		}
		for (int i = 0; i < 2; ++i) delete[] polygons[i];
		delete[] polygons;
		/*std::cout << std::endl;
		for (size_t i = 0; i < 2; ++i) {
			std::cout << "Polygon " << i << ":" << std::endl;
			result[i].display();
		}
		std::cout << std::endl;*/
		return result;
	}
	if (pointsOutNum == 2) {

	}
	if (pointsOutNum == 3) {

	}
	size = 1;
	result = new Polygon3P[1];
	/*std::cout << "Display before the polygon";
	polygon.display();*/
	result[0] = polygon;
	return result;
}
