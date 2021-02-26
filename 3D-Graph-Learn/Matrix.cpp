using namespace MyGeo;

template <unsigned int M, unsigned int N>
Matrix<M, N>::Matrix() {
	std::cout << "Counstructor: " << this << std::endl;
	for (int i = 0; i < M * N; ++i) {
		m_values[i] = 0;
	}
}

template <unsigned int M, unsigned int N>
void Matrix<M, N>::display() {
	int size = M * N;
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			std::cout << m_values[i + j * M] << ' ';
		}
		std::cout << std::endl;
	}
}

template <unsigned int M, unsigned int N>
Matrix<M, N>& Matrix<M, N>::operator=(const Matrix<M, N>& mat) {
	std::cout << "Copy assigment: " << this << std::endl;
	int size = M * N;
	for (int i = 0; i < size; ++i) {
		this->m_values[i] = mat[i];
	}
	return *this;
}

template <unsigned int M, unsigned int N>
float& Matrix<M, N>::operator[](const int& index) {
	return m_values[index];
}

template <unsigned int M, unsigned int N>
float& Matrix<M, N>::operator[](int&& index) {
	return m_values[index];
}

template <unsigned int M, unsigned int N>
const float& Matrix<M, N>::operator[](const int& index) const {
	return m_values[index];
}

template <unsigned int M, unsigned int N>
const float& Matrix<M, N>::operator[](int&& index) const {
	return m_values[index];
}

template <unsigned int M, unsigned int N>
float& Matrix<M, N>::operator()(const int& mIndex, const int& nIndex) {
	return m_values[mIndex + M * nIndex];
}

template <unsigned int M, unsigned int N>
float& Matrix<M, N>::operator()(int&& mIndex, int&& nIndex) {
	return m_values[mIndex + M * nIndex];
}

template <unsigned int M, unsigned int N>
Matrix<M, N>& MyGeo::operator+(Matrix<M, N> A, const Matrix<M, N>& B) {
	int size = M * N;
	for (int i = 0; i < size; ++i) {
		A[i] += B[i];
	}
	return A;
}

template <unsigned int M, unsigned int N>
Matrix<M, N>& MyGeo::operator-(Matrix<M, N> A, const Matrix<M, N>& B) {
	int size = M * N;
	for (int i = 0; i < size; ++i) {
		A[i] -= B[i];
	}
	return A;
}

template <unsigned int M, unsigned int N>
Matrix<M, N>& MyGeo::operator*(Matrix<M, N> mat, const float& value) {
	unsigned int size = M * N;
	for (int i = 0; i < size; ++i) {
		mat[i] *= value;
	}
	return mat;
}

template <unsigned int M, unsigned int N>
Matrix<M, N>& MyGeo::operator*(const float& value, Matrix<M, N> mat) {
	unsigned int size = M * N;
	for (int i = 0; i < size; ++i) {
		mat[i] *= value;
	}
	return mat;
}

template <unsigned int M, unsigned int N>
Matrix<N, M> Matrix<M, N>::transpose() {
	Matrix<N, M> result;
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			result[j + i * N] = m_values[i + j * M];
		}
	}
	return result;
}

template <unsigned int M, unsigned int N>
float Matrix<M, N>::det() {
	if (N != M) return 0;
	return calcDet(m_values, M, N);
}

template <unsigned int M, unsigned int N>
float Matrix<M, N>::calcDet(float* values, const int& sizeX, const int& sizeY) {
	if (sizeX != sizeY) return 0;
	if (sizeX == 1) return values[0];
	if (sizeX == 2) return values[0] * values[3] - values[1] * values[2];
	float sum = 0;
	int subXSize = sizeX - 1;
	int subYSize = sizeY - 1;
	for (int i = 0; i < sizeX; ++i) {
		int size = subXSize * subYSize;
		float* submatrix = new float[size];
		for (int j = 0; j < sizeX; ++j) {
			if (j == i) continue;
			int jVal = j > i ? j - 1 : j;
			for (int k = 1; k < sizeY; ++k) {
				int currSub = jVal + (k - 1) * subYSize;
				submatrix[currSub] = values[j + k * sizeY];
			}
		}
		int coeff = i % 2 == 0 ? 1 : -1;
		sum += coeff * values[i] * calcDet(submatrix, subXSize, subYSize);
		delete[] submatrix;
	}
	return sum;
}