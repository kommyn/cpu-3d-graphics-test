#pragma once

#include "Vector.h"

// TODO: Change namespace name to something cool
namespace vgu {
	// Matrix class prototype
	template <unsigned int M, unsigned int N>
	class Matrix;

	// Operators prototypes
	template <unsigned int M, unsigned int N>
	Matrix<M, N>& operator+(Matrix<M, N> A, const Matrix<M, N>& B);

	template <unsigned int M, unsigned int N>
	Matrix<M, N>& operator-(Matrix<M, N> A, const Matrix<M, N>& B);

	template <unsigned int M, unsigned int N>
	Matrix<M, N>& operator*(Matrix<M, N> mat, const float& value);

	template <unsigned int M, unsigned int N>
	Matrix<M, N>& operator*(const float& value, Matrix<M, N> mat);

	template <unsigned int M, unsigned int N>
	Vector<N> operator*(const Vector<M>& vec, const Matrix<M, N>& mat);

	template <unsigned int M, unsigned int N>
	Vector<M> operator*(const Matrix<M, N>& mat, const Vector<N>& vec);

	// Class itself prototype
	template <unsigned int M, unsigned int N>
	class Matrix {
	private:
		float calcDet(float* values, const int& sizeX, const int& sizeY);
	public:
		float m_values[M * N];

		Matrix();
		Matrix(const float& value);
		~Matrix() = default;

		// TODO: Find a way to move this constructor to template file
		template <typename ...T>
		Matrix(T... data) {
			const unsigned int size = M * N;
			const float arr[size]{ static_cast<float>(data)... };
			for (int i = 0; i < size; ++i) {
				m_values[i / N + M * (i % N)] = arr[i];
			}
		}

		Matrix<M, N>& operator=(const Matrix<M, N>& mat);
		float& operator[](const int& index);
		float& operator[](int&& index);
		const float& operator[](const int& index) const;
		const float& operator[](int&& index) const;
		float& operator()(const int& mIndex, const int& nIndex);
		float& operator()(int&& mIndex, int&& nIndex);
		const float& operator()(const int& mIndex, const int& nIndex) const;
		const float& operator()(int&& mIndex, int&& nIndex) const;
		friend Matrix<M, N>& operator+ <> (Matrix<M, N> A, const Matrix<M, N>& B);
		friend Matrix<M, N>& operator- <>(Matrix<M, N> A, const Matrix<M, N>& B);
		friend Matrix<M, N>& operator* <>(Matrix<M, N> mat, const float& value);
		friend Matrix<M, N>& operator* <>(const float& value, Matrix<M, N> mat);
		friend Vector<N> operator* <>(const Vector<M>& vec, const Matrix<M, N>& mat);
		friend Vector<M> operator* <>(const Matrix<M, N>& mat, const Vector<N>& vec);
		Matrix<N, N> operator-() const;
		Matrix<M, N>& operator+=(const Matrix<M, N>& mat);
		Matrix<M, N>& operator-=(const Matrix<M, N>& mat);
		Matrix<M, N>& operator*=(const float& value);
		Matrix<M, N>& operator/=(const float& value);

		template <unsigned int U>
		Matrix<M, U>& operator*=(const Matrix<N, U>& matrix) {
			const unsigned int size = M * N;
			float tempValues[size];
			for (int i = 0; i < size; ++i) {
				tempValues[i] = m_values[i];
				m_values[i] = 0;
			}
			for (int i = 0; i < M; ++i) {
				for (int j = 0; j < U; ++j) {
					int currentIndex = i + j * M;
					for (int k = 0; k < N; ++k) {
						m_values[currentIndex] += tempValues[i + k * M] * matrix[k + j * N];
					}
				}
			}
			return *this;
		}

		// TODO: Find a way to move this overloaded operator to template file
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

		// TODO: Think about moving of this method to GrapUtils.h
		Matrix<N, M> transpose() const;
		// TODO: Think about moving of this method to GrapUtils.h
		float det() const;
		void display() const;
	};

#include "Matrix.cpp"

#define Matrix2x2 Matrix<2, 2>
#define Matrix3x3 Matrix<3, 3>
#define Matrix4x4 Matrix<4, 4>
}