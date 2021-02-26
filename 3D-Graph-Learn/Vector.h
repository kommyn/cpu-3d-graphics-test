#pragma once

#include <iostream>

// TODO: Change namespace name to something cool
namespace MyGeo {
	template <unsigned char N>
	class Vector;

	template <unsigned char N>
	Vector<N>& operator+(Vector<N> A, const Vector<N>& B);

	template <unsigned char N>
	Vector<N>& operator-(Vector<N> A, const Vector<N>& B);

	template <unsigned char N>
	Vector<N>& operator*(Vector<N> A, const float& value);

	template <unsigned char N>
	Vector<N>& operator*(const float& value, Vector<N> A);

	template <unsigned char N>
	class Vector {
	private:
		float m_coords[N];
	public:
		Vector();
		Vector(int&& val) {
			std::cout << "val: " << val << std::endl;
		}
		template<typename... U,
			typename = typename std::enable_if<sizeof...(U) - 1>::type>
			Vector(U&&... values)
		{
			static_assert(sizeof...(values) == N, "wrong size");
			std::array<int, N> data{ values... };
			for (auto item : data) {
				std::cout << item << std::endl;
			}
		}
		/*Vector(std::initializer_list<float> list) {
			for (int i = 0; i < list.size(); ++i) {
				m_coords[i] = *(list.begin() + i);
			}
		}*/
		~Vector() = default;

		float& operator[](const int& index);
		float& operator[](int&& index);
		const float& operator[](const int& index) const;
		const float& operator[](int&& index) const;

		friend Vector<N>& operator+ <>(Vector<N> A, const Vector<N>& B);
		friend Vector<N>& operator- <>(Vector<N> A, const Vector<N>& B);
		friend Vector<N>& operator* <>(Vector<N> A, const float& value);
		friend Vector<N>& operator* <>(const float& value, Vector<N> A);

		float dot(const Vector<N>& a);
		void display();
		Vector<N> cross(const Vector<N>& a);
	};

#include "Vector.cpp"

#define Vector2f Vector<2>
#define Vector3f Vector<3>
#define Vector4f Vector<4>
}