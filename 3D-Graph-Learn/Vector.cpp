using namespace e3Dg;

template <unsigned char N>
Vector<N>::Vector() {
	for (int i = 0; i < N; ++i) {
		m_coords[i] = 0;
	}
}

template <unsigned char N>
void Vector<N>::display() const {
	std::cout << "(";
	for (int i = 0; i < N; ++i) {
		std::cout << m_coords[i];
		if (i != N - 1) std::cout << ", ";
	}
	std::cout << ");\n";
}

template <unsigned char N>
float& Vector<N>::operator[](const int& index) {
	return m_coords[index];
}

template <unsigned char N>
float& Vector<N>::operator[](int&& index) {
	return m_coords[index];
}

template <unsigned char N>
const float& Vector<N>::operator[](const int& index) const {
	return m_coords[index];
}

template <unsigned char N>
const float& Vector<N>::operator[](int&& index) const {
	return m_coords[index];
}

template <unsigned char N>
Vector<N>& e3Dg::operator+(Vector<N> A, const Vector<N>& B) {
	for (int i = 0; i < N; ++i) A[i] += B[i];
	return A;
}

template <unsigned char N>
Vector<N>& e3Dg::operator-(Vector<N> A, const Vector<N>& B) {
	for (int i = 0; i < N; ++i) A[i] -= B[i];
	return A;
}

template <unsigned char N>
Vector<N>& e3Dg::operator*(Vector<N> A, const float& value) {
	for (int i = 0; i < N; ++i) A[i] *= value;
	return A;
}

template <unsigned char N>
Vector<N>& e3Dg::operator*(const float& value, Vector<N> A) {
	for (int i = 0; i < N; ++i) A[i] *= value;
	return A;
}

template <unsigned char N>
Vector<N>& e3Dg::operator/(Vector<N> A, const float& value) {
	for (int i = 0; i < N; ++i) A[i] /= value;
	return A;
}

template <unsigned char N>
float Vector<N>::dot(const Vector<N>& a) const {
	float dotProduct = 0;
	for (int i = 0; i < N; ++i) dotProduct += m_coords[i] * a[i];
	return dotProduct;
}

template <unsigned char N>
Vector<N>& Vector<N>::operator=(std::initializer_list<float> list) {
	const unsigned int size = list.size() < N ? list.size() : N;
	for (int i = 0; i < size; ++i) {
		this->m_coords[i] = *(list.begin() + i);
	}
	for (int i = size; i < N; ++i) {
		this->m_coords[i] = 0;
	}
	return *this;
}
