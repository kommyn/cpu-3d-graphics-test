#include "Engine3D.h"

Engine3D::Engine3D() {
	//m_model.LoadModel("models/cottage/OBJ/cottage_obj.obj");
	m_model.LoadModel("models/Izba/OBJ/Farmhouse OBJ.obj");
	m_zBuffer = NULL;
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

void Engine3D::FillHalfTriangle(const Pixel& a, const Pixel& b, const float coeffs[3], const double coeefsD[3]) {
	const float longestA = coeffs[0];
	const float longestB = coeffs[1];
	const float lightIntensivity = coeffs[2];
	const double zBuffA = coeefsD[0];
	const double zBuffB = coeefsD[1];
	const double zBuffC = coeefsD[2];

	const int segmentHeight = b.y - a.y;

	if (segmentHeight != 0) {
		float segmentA = (float)(b.x - a.x) / segmentHeight;
		float segmentB = a.x - segmentA * a.y;

		for (int y = a.y; y <= b.y; ++y) {
			int xMin = static_cast<int>(segmentA * y + segmentB + 0.5f);
			int xMax = static_cast<int>(longestA * y + longestB + 0.5f);
			if (xMin > xMax) std::swap(xMin, xMax);
			for (int x = xMin; x <= xMax; ++x) {
				const double zBuffData = zBuffA * x + zBuffB * y + zBuffC;
				// TODO: Fix problem with types in here
				const unsigned long long zBuffPixelIndex = (long long)x + m_pixelsWNum * (long long)y;
				if (m_zBuffer[zBuffPixelIndex] > zBuffData) {
					const unsigned char color = (unsigned char)(lightIntensivity * 230);
					SetPixel({ x, y, { color , color, color, 255} });
					m_zBuffer[zBuffPixelIndex] = zBuffData;
				}
			}
		}
	}
}

vgu::Vector3f lightDir = vgu::normalize(vgu::Vector3f{1.0f, -1.0f, 0.0f});

// TODO: Rewrite this to more optimised, I don't like how it's working right now;
void Engine3D::FillTriangle(const Polygon3P& polygon) {
	vgu::Vector4f A = polygon.m_first, B = polygon.m_second, C = polygon.m_third;
	A[0] = static_cast<int>(A[0] * m_pixelsWNum + m_pixelsWNum) * 0.5f;
	A[1] = static_cast<int>(A[1] * m_pixelsHNum + m_pixelsHNum) * 0.5f;
	B[0] = static_cast<int>(B[0] * m_pixelsWNum + m_pixelsWNum) * 0.5f;
	B[1] = static_cast<int>(B[1] * m_pixelsHNum + m_pixelsHNum) * 0.5f;
	C[0] = static_cast<int>(C[0] * m_pixelsWNum + m_pixelsWNum) * 0.5f;
	C[1] = static_cast<int>(C[1] * m_pixelsHNum + m_pixelsHNum) * 0.5f;

	Pixel a = { static_cast<int>(A[0]), static_cast<int>(A[1]) };
	Pixel b = { static_cast<int>(B[0]), static_cast<int>(B[1]) };
	Pixel c = { static_cast<int>(C[0]), static_cast<int>(C[1]) };

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

	const double B0 = static_cast<double>(B[0]);
	const double B1 = static_cast<double>(B[1]);
	const double B2 = static_cast<double>(B[2]);
	const double A0 = static_cast<double>(A[0]);
	const double A1 = static_cast<double>(A[1]);
	const double A2 = static_cast<double>(A[2]);
	const double C0 = static_cast<double>(C[0]);
	const double C1 = static_cast<double>(C[1]);
	const double C2 = static_cast<double>(C[2]);
	// TODO: It is just trying to do calculations on double precision numbers for the W-buffer (I should fix it to something correct)
	const double delta = (B0 - A0) * (C1 - A1) - (C0 - A0) * (B1 - A1);
	const double zBuffA = ((B2 - A2) * (C1 - A1) - (C2 - A2) * (B1 - A1)) / delta;
	const double zBuffB = ((B0 - A0) * (C2 - A2) - (C0 - A0) * (B2 - A2)) / delta;
	const double zBuffC = (A0 * (B1 * C2 - C1 * B2) - B0 * (A1 * C2 - C1 * A2) + C0 * (A1 * B2 - B1 * A2)) / delta;

	const float lightIntensivity = (1 - vgu::dotProduct(lightDir, polygon.m_normal)) * 0.5f;

	const float longestA = (float)(c.x - a.x) / (c.y - a.y);
	const float longestB = a.x - longestA * a.y;
	const float coeffs[3] = { longestA, longestB, lightIntensivity };
	const double coeefsD[3] = { zBuffA, zBuffB, zBuffC };

	FillHalfTriangle(a, b, coeffs, coeefsD);
	FillHalfTriangle(b, c, coeffs, coeefsD);
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
	A[0] = static_cast<int>(A[0] * m_pixelsWNum + m_pixelsWNum) * 0.5f;
	A[1] = static_cast<int>(A[1] * m_pixelsHNum + m_pixelsHNum) * 0.5f;
	B[0] = static_cast<int>(B[0] * m_pixelsWNum + m_pixelsWNum) * 0.5f;
	B[1] = static_cast<int>(B[1] * m_pixelsHNum + m_pixelsHNum) * 0.5f;
	C[0] = static_cast<int>(C[0] * m_pixelsWNum + m_pixelsWNum) * 0.5f;
	C[1] = static_cast<int>(C[1] * m_pixelsHNum + m_pixelsHNum) * 0.5f;
	
	Pixel a = { static_cast<int>(A[0]), static_cast<int>(A[1]) };
	Pixel b = { static_cast<int>(B[0]), static_cast<int>(B[1]) };
	Pixel c = { static_cast<int>(C[0]), static_cast<int>(C[1]) };
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

	const double B0 = static_cast<double>(B[0]);
	const double B1 = static_cast<double>(B[1]);
	const double B2 = static_cast<double>(B[2]);
	const double A0 = static_cast<double>(A[0]);
	const double A1 = static_cast<double>(A[1]);
	const double A2 = static_cast<double>(A[2]);
	const double C0 = static_cast<double>(C[0]);
	const double C1 = static_cast<double>(C[1]);
	const double C2 = static_cast<double>(C[2]);
	const double delta = (B0 - A0) * (C1 - A1) - (C0 - A0) * (B1 - A1);
	if (std::abs(delta) < 0.00001f) return;
	const double zBuffA = ((B2 - A2) * (C1 - A1) - (C2 - A2) * (B1 - A1)) / delta;
	const double zBuffB = ((B0 - A0) * (C2 - A2) - (C0 - A0) * (B2 - A2)) / delta;
	const double zBuffC = (A0 * (B1 * C2 - C1 * B2) - B0 * (A1 * C2 - C1 * A2) + C0 * (A1 * B2 - B1 * A2)) / delta;
	/*const vgu::Matrix2x2 deltaMatrix = {
		B[0] - A[0], B[1] - A[1],
		C[0] - A[0], C[1] - A[1]
	};
	const float delta = vgu::det(deltaMatrix);
	if (std::abs(delta) < 0.00001f) return;
	const vgu::Matrix2x2 zBuffAMat = {
		B[2] - A[2], B[1] - A[1],
		C[2] - A[2], C[1] - A[1]
	};
	const float zBuffA = vgu::det(zBuffAMat) / delta;
	const vgu::Matrix2x2 zBuffBMat = {
		B[0] - A[0], B[2] - A[2],
		C[0] - A[0], C[2] - A[2]
	};
	const float zBuffB = vgu::det(zBuffBMat) / delta;
	const vgu::Matrix3x3 zBuffCMat = {
		A[0], A[1], A[2],
		B[0], B[1], B[2],
		C[0], C[1], C[2],
	};
	const float zBuffC = vgu::det(zBuffCMat) / delta;*/

	const double coeffs[3] = { zBuffA, zBuffB, zBuffC };

	DrawLine(a, b, coeffs);
	DrawLine(b, c, coeffs);
	DrawLine(c, a, coeffs);
}