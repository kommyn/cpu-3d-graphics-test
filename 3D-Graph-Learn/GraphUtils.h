#pragma once

#include "Matrix.h"

// TODO: Create util functions for matrix base transformations (rotation, translation, scaling and something else)
// TODO: There will be a great idea to add another template argument for Vector and Matrix classes that defines Vector/Matrix
//       element data type (like float, double, int, or something else). Default value for this argument will be float
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

	template <unsigned char N = 2>
	float det(const Matrix<2, 2>& mat) {
		return mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0);
	}

	template <unsigned char N = 3>
	float det(const Matrix<3, 3>& mat) {
		const float firstSum = mat(0, 0) * mat(1, 1) * mat(2, 2) + mat(0, 1) * mat(1, 2) * mat(2, 0) + mat(1, 0) * mat(2, 1) * mat(0, 2);
		const float secondSum = mat(0, 2) * mat(1, 1) * mat(2, 0) + mat(1, 0) * mat(0, 1) * mat(2, 2) + mat(2, 1) * mat(1, 2) * mat(0, 0);
		return firstSum - secondSum;
	}

	Vector3f crossProduct(const Vector3f& A, const Vector3f& B);

	Vector4f vecToHomogen(const Vector3f& a);

	Vector3f vecToEuclid(const Vector4f& a);
}