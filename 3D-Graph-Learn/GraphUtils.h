#pragma once

#include "Matrix.h"

// TODO: Maybe it is a good idea to make this file main node of all graph files
namespace e3Dg {
	template <unsigned char N>
	float dotProduct(const Vector<N>& A, const Vector<N>& B) {
		float sum = 0;
		for (int i = 0; i < N; ++i) sum += A[i] * B[i];
		return sum;
	}

	Vector3f crossProduct(const Vector3f& A, const Vector3f& B);

	Vector4f vecToHomogen(const Vector3f& a);

	Vector3f vecToEuclid(const Vector4f& a);
}