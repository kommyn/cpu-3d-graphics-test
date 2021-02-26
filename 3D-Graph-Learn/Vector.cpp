using namespace MyGeo;

template <unsigned char N>
Vector<N>::Vector() {
	for (int i = 0; i < N; ++i) {
		m_coords[i] = 0;
	}
}

template <unsigned char N>
void Vector<N>::display() {
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
Vector<N>& MyGeo::operator+(Vector<N> A, const Vector<N>& B) {
	for (int i = 0; i < N; ++i) A[i] += B[i];
	return A;
}

template <unsigned char N>
Vector<N>& MyGeo::operator-(Vector<N> A, const Vector<N>& B) {
	for (int i = 0; i < N; ++i) A[i] -= B[i];
	return A;
}

template <unsigned char N>
Vector<N>& MyGeo::operator*(Vector<N> A, const float& value) {
	for (int i = 0; i < N; ++i) A[i] *= value;
	return A;
}

template <unsigned char N>
Vector<N>& MyGeo::operator*(const float& value, Vector<N> A) {
	for (int i = 0; i < N; ++i) A[i] *= value;
	return A;
}

template <unsigned char N>
float Vector<N>::dot(const Vector<N>& a) {
	float dotProduct = 0;
	for (int i = 0; i < N; ++i) dotProduct += m_coords[i] * a[i];
	return dotProduct;
}

template <unsigned char N>
Vector<N> Vector<N>::cross(const Vector<N>& a) {
	Vector<N> result;
	if (N != 3) return result;
	result[0] = m_coords[1] * a[2] - m_coords[2] * a[1];
	result[1] = m_coords[2] * a[0] - m_coords[0] * a[2];
	result[2] = m_coords[0] * a[1] - m_coords[1] * a[0];
	return result;
}