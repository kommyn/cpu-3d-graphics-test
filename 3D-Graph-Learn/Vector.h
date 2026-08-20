#pragma once

#include <iostream>

namespace vgu {
	template <unsigned char N>
	class Vector;

	// Just a bunch of operator prototypes
	template <unsigned char N>
	Vector<N> operator+(Vector<N> A, const Vector<N>& B);

	template <unsigned char N>
	Vector<N> operator-(Vector<N> A, const Vector<N>& B);

	template <unsigned char N>
	Vector<N> operator*(Vector<N> A, const double& value);

	template <unsigned char N>
	Vector<N> operator*(const double& value, Vector<N> A);

	template <unsigned char N>
	Vector<N> operator/(Vector<N> A, const double& value);

	// It is coords template structure that are used for using in union of data with vectors of size 1, 2, 3, 4
	template <unsigned int N>
	struct Coords {};

	template <>
	struct Coords<1> {
		double value;
	};

	template <>
	struct Coords<2> {
		double x, y;
	};

	template <>
	struct Coords<3> {
		double x, y, z;
	};

	template <>
	struct Coords<4> {
		double x, y, z, w;
	};

	// Vector class itself
	template <unsigned char N>
	class Vector {
	public:
		union {
			double m_coords[N];
			Coords<N> coord;
		};

		Vector();
		~Vector() = default;

		// TODO: Find a way how to move this constructor to template file
		template <typename ...T>
		Vector(T... data) : m_coords{ static_cast<double>(data)... } {}

		// TODO: Think about adding of *= operator for Vector*Matrix multiplication
		double& operator[](const int& index);
		double& operator[](int&& index);
		const double& operator[](const int& index) const;
		const double& operator[](int&& index) const;
		Vector<N>& operator=(std::initializer_list<double> list);
		friend Vector<N> operator+ <>(Vector<N> A, const Vector<N>& B);
		friend Vector<N> operator- <>(Vector<N> A, const Vector<N>& B);
		friend Vector<N> operator* <>(Vector<N> A, const double& value);
		friend Vector<N> operator* <>(const double& value, Vector<N> A);
		friend Vector<N> operator/ <>(Vector<N> A, const double& value);
		Vector<N>& operator+=(const Vector<N>& vector);
		Vector<N>& operator-=(const Vector<N>& vector);
		Vector<N>& operator*=(const double& value);
		Vector<N>& operator/=(const double& value);
		Vector<N> operator-() const;

		double length() const;
		void display() const;
	};

#include "Vector.cpp"

#define Vector2f Vector<2>
#define Vector3f Vector<3>
#define Vector4f Vector<4>
}