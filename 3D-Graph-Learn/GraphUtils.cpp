#include "GraphUtils.h"

using namespace vgu;

Vector3f vgu::crossProduct(const Vector3f& A, const Vector3f& B) {
	Vector3f result;
	result[0] = A[1] * B[2] - A[2] * B[1];
	result[1] = A[2] * B[0] - A[0] * B[2];
	result[2] = A[0] * B[1] - A[1] * B[0];
	return result;
}

Vector4f vgu::vecToHomogen(const Vector3f& A) {
	return { A[0], A[1], A[2], 1};
}

Vector3f vgu::vecToEuclid(const Vector4f& A) {
	if (A[3] == 0) return { A[0], A[1], A[2] };
	return { A[0] / A[3], A[1] / A[3], A[2] / A[3] };
}

float vgu::calcDetRecursively(const float values[], const int& size) {
	if (size == 1) return values[0];
	if (size == 2) return values[0] * values[3] - values[1] * values[2];
	float sum = 0;
	int subSize = size - 1;
	int subArrSize = subSize * subSize;
	for (int i = 0; i < size; ++i) {
		float* submatrix = new float[subArrSize];
		for (int j = 0; j < size; ++j) {
			if (j == i) continue;
			int jVal = j > i ? j - 1 : j;
			for (int k = 1; k < size; ++k) {
				int currSub = jVal + (k - 1) * subSize;
				submatrix[currSub] = values[j + k * size];
			}
		}
		int coeff = i % 2 == 0 ? 1 : -1;
		sum += coeff * values[i] * calcDetRecursively(submatrix, subSize);
		delete[] submatrix;
	}
	return sum;
}