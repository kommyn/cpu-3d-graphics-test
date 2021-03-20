#pragma once

#include <iostream>

namespace e3Dg {
	template <unsigned char N>
	class Vector;

	// Just a bunch of operator prototypes
	template <unsigned char N>
	Vector<N>& operator+(Vector<N> A, const Vector<N>& B);

	template <unsigned char N>
	Vector<N>& operator-(Vector<N> A, const Vector<N>& B);

	template <unsigned char N>
	Vector<N>& operator*(Vector<N> A, const float& value);

	template <unsigned char N>
	Vector<N>& operator*(const float& value, Vector<N> A);

	template <unsigned char N>
	Vector<N>& operator/(Vector<N> A, const float& value);

	// It is coords template structure that are used for using in union of data with vectors of size 1, 2, 3, 4
	template <unsigned int N>
	struct Coords {};

	template <>
	struct Coords<1> {
		float value;
	};

	template <>
	struct Coords<2> {
		float x, y;
	};

	template <>
	struct Coords<3> {
		float x, y, z;
	};

	template <>
	struct Coords<4> {
		float x, y, z, w;
	};

	// Vector class itself
	template <unsigned char N>
	class Vector {
	public:
		union {
			float m_coords[N];
			Coords<N> coord;
		};

		Vector();
		~Vector() = default;

		// TODO: Find a way how to move this constructor to template file
		template <typename ...T>
		Vector(T... data) : m_coords{ static_cast<float>(data)... } {
		}

		// TODO: Think about adding of *= operator for Vector*Matrix multiplication
		float& operator[](const int& index);
		float& operator[](int&& index);
		const float& operator[](const int& index) const;
		const float& operator[](int&& index) const;
		Vector<N>& operator=(std::initializer_list<float> list);
		friend Vector<N>& operator+ <>(Vector<N> A, const Vector<N>& B);
		friend Vector<N>& operator- <>(Vector<N> A, const Vector<N>& B);
		friend Vector<N>& operator* <>(Vector<N> A, const float& value);
		friend Vector<N>& operator* <>(const float& value, Vector<N> A);
		friend Vector<N>& operator/ <>(Vector<N> A, const float& value);
		Vector<N>& operator+=(const Vector<N>& vector);
		Vector<N>& operator-=(const Vector<N>& vector);
		Vector<N>& operator*=(const float& value);
		Vector<N>& operator/=(const float& value);
		Vector<N>& operator-() const;

		// TODO: Think about moving of this method to GrapUtils.h
		float dot(const Vector<N>& a) const;
		float length() const;
		void display() const;
	};

#include "Vector.cpp"

#define Vector2f Vector<2>
#define Vector3f Vector<3>
#define Vector4f Vector<4>
}