using namespace vgu;

///////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////

template <unsigned int M, unsigned int N>
Matrix<M, N>::Matrix() {
	for (int i = 0; i < M * N; ++i) {
		m_values[i] = 0;
	}
}

template <unsigned int M, unsigned int N>
Matrix<M, N>::Matrix(const double& value) {
	const unsigned int size = M * N;
	for (int i = 0; i < size; ++i) {
		m_values[i] = value;
	}
}

///////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////

template <unsigned int M, unsigned int N>
void Matrix<M, N>::display() const {
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
	int size = M * N;
	for (int i = 0; i < size; ++i) {
		this->m_values[i] = mat[i];
	}
	return *this;
}

template <unsigned int M, unsigned int N>
double& Matrix<M, N>::operator[](const int& index) {
	return m_values[index];
}

template <unsigned int M, unsigned int N>
double& Matrix<M, N>::operator[](int&& index) {
	return m_values[index];
}

template <unsigned int M, unsigned int N>
const double& Matrix<M, N>::operator[](const int& index) const {
	return m_values[index];
}

template <unsigned int M, unsigned int N>
const double& Matrix<M, N>::operator[](int&& index) const {
	return m_values[index];
}

template <unsigned int M, unsigned int N>
double& Matrix<M, N>::operator()(const int& mIndex, const int& nIndex) {
	return m_values[mIndex + M * nIndex];
}

template <unsigned int M, unsigned int N>
double& Matrix<M, N>::operator()(int&& mIndex, int&& nIndex) {
	return m_values[mIndex + M * nIndex];
}

template <unsigned int M, unsigned int N>
const double& Matrix<M, N>::operator()(const int& mIndex, const int& nIndex) const {
	return m_values[mIndex + M * nIndex];
}

template <unsigned int M, unsigned int N>
const double& Matrix<M, N>::operator()(int&& mIndex, int&& nIndex) const {
	return m_values[mIndex + M * nIndex];
}

template <unsigned int M, unsigned int N>
Matrix<M, N>& vgu::operator+(Matrix<M, N> A, const Matrix<M, N>& B) {
	int size = M * N;
	for (int i = 0; i < size; ++i) {
		A[i] += B[i];
	}
	return A;
}

template <unsigned int M, unsigned int N>
Matrix<M, N>& vgu::operator-(Matrix<M, N> A, const Matrix<M, N>& B) {
	int size = M * N;
	for (int i = 0; i < size; ++i) {
		A[i] -= B[i];
	}
	return A;
}

template <unsigned int M, unsigned int N>
Matrix<M, N>& vgu::operator*(Matrix<M, N> mat, const double& value) {
	unsigned int size = M * N;
	for (int i = 0; i < size; ++i) {
		mat[i] *= value;
	}
	return mat;
}

template <unsigned int M, unsigned int N>
Matrix<M, N>& vgu::operator*(const double& value, Matrix<M, N> mat) {
	unsigned int size = M * N;
	for (int i = 0; i < size; ++i) {
		mat[i] *= value;
	}
	return mat;
}

template <unsigned int M, unsigned int N>
Vector<N> vgu::operator*(const Vector<M>& vec, const Matrix<M, N>& mat) {
	Vector<N> result;
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			result[j] += vec[i] * mat[i + M * j];
		}
	}
	return result;
}

template <unsigned int M, unsigned int N>
Vector<M> vgu::operator*(const Matrix<M, N>& mat, const Vector<N>& vec) {
	Vector<M> result;
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			result[i] += vec[j] * mat[i + M * j];
		}
	}
	return result;
}

template <unsigned int M, unsigned int N>
Matrix<N, N> Matrix<M,N>::operator-() const {
	Matrix<M, N> result;
	const unsigned int size = M * N;
	for (unsigned int i = 0; i < size; ++i) {
		result[i] = - m_values[i];
	}
	return result;
}

template <unsigned int M, unsigned int N>
Matrix<M, N>& Matrix<M, N>::operator+=(const Matrix<M, N>& mat) {
	const unsigned int size = M * N;
	for (unsigned int i = 0; i < size; ++i) {
		m_values[i] += mat[i];
	}
	return *this;
}

template <unsigned int M, unsigned int N>
Matrix<M, N>& Matrix<M, N>::operator-=(const Matrix<M, N>& mat) {
	const unsigned int size = M * N;
	for (unsigned int i = 0; i < size; ++i) {
		m_values[i] -= mat[i];
	}
	return *this;
}

template <unsigned int M, unsigned int N>
Matrix<M, N>& Matrix<M, N>::operator*=(const double& value) {
	const unsigned int size = M * N;
	for (unsigned int i = 0; i < size; ++i) {
		m_values[i] *= value;
	}
	return *this;
}

template <unsigned int M, unsigned int N>
Matrix<M, N>& Matrix<M, N>::operator/=(const double& value) {
	const unsigned int size = M * N;
	for (unsigned int i = 0; i < size; ++i) {
		m_values[i] /= value;
	}
	return *this;
}

template <unsigned int M, unsigned int N>
Matrix<N, M> Matrix<M, N>::transpose() const {
	Matrix<N, M> result;
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			result[j + i * N] = m_values[i + j * M];
		}
	}
	return result;
}
