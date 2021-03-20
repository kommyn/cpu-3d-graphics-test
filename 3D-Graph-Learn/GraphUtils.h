#pragma once

#include "Matrix.h"

// TODO: Maybe it is a good idea to make this file main node of all graph files
namespace vgu {
	template <unsigned char N>
	float dotProduct(const Vector<N>& A, const Vector<N>& B) {
		float sum = 0;
		for (int i = 0; i < N; ++i) sum += A[i] * B[i];
		return sum;
	}

	template <unsigned char N>
	Vector<N> normalize(const Vector<N>& A) {
		return A / A.length();
	}

	float calcDetRecursively(const float values[], const int& size);

	template <unsigned char N>
	float det(const Matrix<N, N>& mat) {
		return calcDetRecursively(mat.m_values, N);
	}

	Vector3f crossProduct(const Vector3f& A, const Vector3f& B);

	Vector4f vecToHomogen(const Vector3f& a);

	Vector3f vecToEuclid(const Vector4f& a);
}