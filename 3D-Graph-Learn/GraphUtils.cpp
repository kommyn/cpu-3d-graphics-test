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