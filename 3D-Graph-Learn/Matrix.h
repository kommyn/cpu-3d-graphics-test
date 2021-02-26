#pragma once

#include "Vector.h"

// TODO: Change namespace name to something cool
namespace MyGeo {
	template <unsigned int M, unsigned int N>
	class Matrix;

	template <unsigned int M, unsigned int N>
	Matrix<M, N>& operator+(Matrix<M, N> A, const Matrix<M, N>& B);

	template <unsigned int M, unsigned int N>
	Matrix<M, N>& operator-(Matrix<M, N> A, const Matrix<M, N>& B);

	template <unsigned int M, unsigned int N>
	Matrix<M, N>& operator*(Matrix<M, N> mat, const float& value);

	template <unsigned int M, unsigned int N>
	Matrix<M, N>& operator*(const float& value, Matrix<M, N> mat);

	template <unsigned int M, unsigned int N>
	class Matrix {
	private:
		float m_values[M * N];

		float calcDet(float* values, const int& sizeX, const int& sizeY);
	public:
		Matrix();
		virtual ~Matrix() = default;

		void display();

		Matrix<M, N>& operator=(const Matrix<M, N>& mat);

		float& operator[](const int& index);
		float& operator[](int&& index);
		const float& operator[](const int& index) const;
		const float& operator[](int&& index) const;
		float& operator()(const int& mIndex, const int& nIndex);
		float& operator()(int&& mIndex, int&& nIndex);

		friend Matrix<M, N>& operator+ <> (Matrix<M, N> A, const Matrix<M, N>& B);
		friend Matrix<M, N>& operator- <>(Matrix<M, N> A, const Matrix<M, N>& B);
		friend Matrix<M, N>& operator* <>(Matrix<M, N> mat, const float& value);
		friend Matrix<M, N>& operator*(const float& value, Matrix<M, N> mat);

		template <unsigned int U>
		friend Matrix<M, U> operator*(const Matrix<M, N>& A, const Matrix<N, U>& B) {
			Matrix<M, U> result;
			for (int i = 0; i < M; ++i) {
				for (int j = 0; j < U; ++j) {
					int currentIndex = i + j * M;
					for (int k = 0; k < N; ++k) {
						result[currentIndex] += A[i + k * M] * B[k + j * N];
					}
				}
			}
			return result;
		}
		friend Vector<N> operator*(const Vector<M>& vec, const Matrix<M, N>& mat) {
			Vector<N> result;
			for (int i = 0; i < M; ++i) {
				for (int j = 0; j < N; ++j) {
					result[j] += vec[i] * mat[i + M * j];
				}
			}
			return result;
		}

		Matrix<N, M> transpose();
		float det();
	};

#include "Matrix.cpp"

#define Matrix2x2 Matrix<2, 2>
#define Matrix3x3 Matrix<3, 3>
#define Matrix4x4 Matrix<4, 4>
}