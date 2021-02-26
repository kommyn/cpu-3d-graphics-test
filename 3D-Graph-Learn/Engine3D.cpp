#include "Engine3D.h"

Engine3D::Engine3D() {
	m_lookTo[0] = 0.0f;
	m_lookTo[1] = 0.0f;
	m_lookTo[2] = -1.0f;
}

void Engine3D::DrawLine(Pixel a, Pixel b) {
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
		float colX = (pointX - (steep ? a.y : a.x)) / (steep ? (float)dy : (float)dx);
		float colY = (pointY - (steep ? a.x : a.y)) / (steep ? (float)dx : (float)dy);
		float percent = std::sqrt((colX * colX + colY * colY) / 2);
		//SetPixel({ pointX, pointY, (unsigned char)(b.r * percent + a.r * (1 - percent)), (unsigned char)(b.g * percent + a.g * (1 - percent)), (unsigned char)(b.b * percent + b.b * (1 - percent)) });
		SetPixel({ pointX, pointY, { 200, 200, 200, 255 } });
		error -= dy;
		if (error < 0) {
			y += yStep;
			error += dx;
		}
	}
}

void Engine3D::DrawTriangle(const Pixel& a, const Pixel& b, const Pixel& c) {
	DrawLine(a, b);
	DrawLine(b, c);
	DrawLine(c, a);
}

void Engine3D::InterpolatedHalfTriangle(const Pixel& a, const Pixel& b, float* coeffs) {
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
			long xMin = segmentA * y + segmentB + 0.5;
			long xMax = longestA * y + longestB + 0.5;
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
				else red = fRed;
				if (fGreen > 255) {
					green = 255;
				}
				else if (fGreen < 0) {
					green = 0;
				}
				else green = fGreen;
				if (fBlue > 255) {
					blue = 255;
				}
				else if (fBlue < 0) {
					blue = 0;
				}
				else blue = fBlue;
				SetPixel({ x, y, { red, green, blue, 255 } });
			}
		}
	}
}

void Engine3D::InterpolatedTriangle(Pixel a, Pixel b, Pixel c) {
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
}

void TexturedHalfTriangle(const Pixel& a, const Pixel& b) {

}

void Engine3D::TexturedTriangle(Pixel a, Pixel b, Pixel c, std::string filePath) {
	Texture* texture = m_texturesFact.LoadTexture(filePath);
	if (!texture) return InterpolatedTriangle(a, b, c);		

	/*if (a.y > b.y) {
		std::swap(a, b);
		std::swap(ta, tb);
	}
	if (a.y > c.y) {
		std::swap(a, c);
		std::swap(ta, tc);
	}
	if (b.y > c.y) {
		std::swap(b, c);
		std::swap(tb, tc);
	}

	float delta = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
	float txA = ((tb.x - ta.x) * (c.y - a.y) - (tc.x - ta.x) * (b.y - a.y)) / delta;
	float txB = ((b.x - a.x) * (tc.x - ta.x) - (c.x - a.x) * (tb.x - ta.x)) / delta;
	float txC = (a.x * (b.y * tc.x - c.y * tb.x) - a.y * (b.x * tc.x - c.x * tb.x) + ta.x * (b.x * c.y - c.x * b.y)) / delta;
	float tyA = ((tb.y - ta.y) * (c.y - a.y) - (tc.y - ta.y) * (b.y - a.y)) / delta;
	float tyB = ((b.x - a.x) * (tc.y - ta.y) - (c.x - a.x) * (tb.y - ta.y)) / delta;
	float tyC = (a.x * (b.y * tc.y - c.y * tb.y) - a.y * (b.x * tc.y - c.x * tb.y) + ta.y * (b.x * c.y - c.x * b.y)) / delta;

	// Coeffitients for the longest part of the triangle
	float longestA = (float)(c.x - a.x) / (c.y - a.y);
	float longestB = a.x - longestA * a.y;

	long secondSegmentHeight = b.y - a.y;
	if (secondSegmentHeight > 0) {
		// Coeffitients for the first line part of the triangle
		float secondSegmentA = (float)(b.x - a.x) / secondSegmentHeight;
		float secondSegmentB = a.x - secondSegmentA * a.y;

		for (long y = a.y; y <= b.y; ++y) {
			long xMin = secondSegmentA * y + secondSegmentB + 0.5;
			long xMax = longestA * y + longestB + 0.5;
			if (xMin > xMax) std::swap(xMin, xMax);
			for (long x = xMin; x <= xMax; ++x) {
				long textureX = x * txA + y * txB + txC;
				long textureY = x * tyA + y * tyB + tyC;
				long textureIndex = (textureX + textureY * img.width) * 4;
				unsigned char red = img.pixels[textureIndex + 2];
				unsigned char green = img.pixels[textureIndex + 1];
				unsigned char blue = img.pixels[textureIndex];
				unsigned char alpha = img.pixels[textureIndex + 3];
				SetPixel({ x, y, { red, green, blue, 255 } });
			}
		}
	}

	// Coeffitients for the second line part of the triangle
	long firstSegmentHeight = c.y - b.y;
	if (firstSegmentHeight > 0) {
		float firstSegmentA = (float)(c.x - b.x) / firstSegmentHeight;
		float firstSegmentB = b.x - firstSegmentA * b.y;

		for (long y = b.y; y <= c.y; ++y) {
			long xMin = firstSegmentA * y + firstSegmentB + 0.5;
			long xMax = longestA * y + longestB + 0.5;
			if (xMin > xMax) std::swap(xMin, xMax);
			for (long x = xMin; x <= xMax; ++x) {
				long textureX = x * txA + y * txB + txC;;
				long textureY = x * tyA + y * tyB + tyC;
				long textureIndex = (textureX + textureY * img.width) * 4;
				unsigned char red = img.pixels[textureIndex + 2];
				unsigned char green = img.pixels[textureIndex + 1];
				unsigned char blue = img.pixels[textureIndex];
				unsigned char alpha = img.pixels[textureIndex + 3];
				SetPixel({ x, y, { red, green, blue, 255 } });
			}
		}
	}*/
}

void Engine3D::VertexPipe() {

}
