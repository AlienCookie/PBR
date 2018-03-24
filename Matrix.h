#ifndef MATRX
#define MATRX

#include<iostream>

template <typename U>
class Matrix
{
public:
	Matrix(int rows_, int columns_);
	Matrix();
	//Use this constructor with explicit type casting
	//e. g. ((int*)a, 3, 3);
	Matrix(U* source, int rows_, int columns_);
	Matrix(U** source, int rows_, int columns_);
	Matrix(const Matrix&);
	Matrix(const Matrix&, int rows, int columns);
	~Matrix();
	void setSize(int rows_, int columns_);
	void print() const;
	Matrix transposed() const;
	int rows() const;
	int columns() const;
	Matrix operator+(Matrix);
	Matrix operator*(Matrix);
	Matrix operator*(U);
	U &operator[] (int i);
	U &operator[] (int i) const;
	Matrix operator =(const Matrix &);
	operator U* ();
	Matrix<U> minor(int row, int col);
	Matrix<U> Matrix<U>::inverse();
	U norm();
	U cond();
	U det();



protected:
	U* matrix = NULL;
	int Rows;
	int Columns;
};

template <typename U>
void Matrix<U>::print() const
{
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Columns; j++)
			std::cout << matrix[i*Columns + j]  << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

template <typename U>
Matrix<U> Matrix<U>::transposed() const
{
	Matrix res(Columns, Rows);
	for (int i = 0; i < Rows; i++)
		for (int j = 0; j < Columns; j++)
			res[i*Columns + j] = matrix[j*Columns + i];
	return res;
}

template<typename U>
inline int Matrix<U>::rows() const
{
	return Rows;
}

template<typename U>
int Matrix<U>::columns() const
{
	return Columns;
}

template <typename U>
Matrix<U>::Matrix(int rows_, int columns_) :
	Rows(rows_), Columns(columns_)
{
	matrix = new U[Rows*Columns]();
}

template<typename U>
Matrix<U>::Matrix() :Rows(0), Columns(0) {}

template <typename U>
Matrix<U>::Matrix(U* source, int rows_, int columns_)
{
	this->Rows = rows_;
	this->Columns = columns_;
	matrix = new U[Rows*Columns];
	for (int i = 0; i < Rows; i++) {
		for (int j = 0; j < Columns; j++)
			matrix[i*Columns + j] = source[i*Columns + j];
	}
}

template<typename U>
Matrix<U>::Matrix(U ** source, int rows_, int columns_)
{
	this->Rows = rows_;
	this->Columns = columns_;
	matrix = new U[Rows*Columns];
	for (int i = 0; i < Rows; i++) {
		for (int j = 0; j < Columns; j++)
			matrix[i*Columns + j] = source[i][j];
	}
}

template<typename U>
Matrix<U>::Matrix(const Matrix &another)
{
	Rows = another.Rows;
	Columns = another.Columns;
	matrix = new U[Rows*Columns];
	for (int i = 0; i < Rows; i++) {
		for (int j = 0; j < Columns; j++)
			matrix[i*Columns + j] = another.matrix[i*Columns + j];
	}
}

template<typename U>
Matrix<U>::Matrix(const Matrix &another, int rows, int columns)
{
	Rows = rows;
	Columns = columns;
	matrix = new U[Rows*Columns];
	for (int i = 0; i < Rows; i++) {
		for (int j = 0; j < Columns; j++)
			matrix[i*Columns + j] = another.matrix[i*another.Columns + j];
	}
}

template<typename U>
Matrix<U>::~Matrix()
{
	delete matrix;
}

template<typename U>
inline void Matrix<U>::setSize(int rows_, int columns_)
{
	Rows = rows_;
	Columns = columns_;
	matrix = new U[Rows*Columns];
}

template <typename U>
Matrix<U> Matrix<U>::operator + (Matrix<U> A)
{
	if (A.Rows == Rows && A.Columns == Columns)
	{
		Matrix<U> C = A;
		for (int i = 0; i < A.Rows; i++)
			for (int j = 0; j < A.Columns; j++)
				C[i*A.Columns + j] = C[i*A.Columns + j] + matrix[i*Columns + j];
		return C;
	}
	else std::cerr << "Error: adding matrices with different dimensions";
}

template <typename U>
Matrix<U> Matrix<U>::operator * (Matrix<U> A)
{
	if (Columns == A.Rows)
	{
		Matrix<U> C(Rows, A.Columns);
		U data = 0.0;
		for (int i = 0; i < Rows; i++)
		{
			for (int j = 0; j < A.Columns; j++)
			{
				for (int r = 0; r < Columns; r++)
					data += matrix[i*Columns + r] * A[r*A.Columns + j];
				C[i*A.Columns + j] = data;
				data = 0;
			}
		}
		return C;
	}
	else std::cerr << "Error: multipling matrices whith inappropriate dimensions";
}

template <typename U>
Matrix<U> Matrix<U>::operator * (U alpha)
{
	Matrix<U> C = *this;
	for (int i = 0; i < Rows; i++)
		for (int j = 0; j < Columns; j++)
			C[i*Columns + j] *= alpha;
	return C;
}

template <typename U>
U & Matrix<U>::operator[] (int i) {
	return matrix[i];
}

template <typename U>
U & Matrix<U>::operator[] (int i) const {
	return matrix[i];
}

template<typename U>
Matrix<U>  Matrix<U>::operator=(const Matrix<U>& another)
{
	Rows = another.Rows;
	Columns = another.Columns;
	if (matrix != NULL) {
		delete matrix;
	}
	matrix = new U[Rows*Columns];
	for (int i = 0; i < Rows; i++) {
		for (int j = 0; j < Columns; j++)
			matrix[i*Columns+j] = another[i*Columns + j];
	}
	return *this;
}

template<typename U>
inline Matrix<U>::operator U*()
{
	return matrix;
}

template<typename U>
Matrix<U> Matrix<U>::inverse()
{
	int order = Rows;
	Matrix<U> Y(order, order);

	U det_A = det();

	for (int j = 0; j < order; j++)
	{
		for (int i = 0; i < order; i++)
		{
			// get the co-factor (matrix) of A(j,i)
			Matrix<U> m = minor(j, i);
			Y[i * order + j] = m.det() / det_A;
			if ((i + j) % 2 == 1)
				Y[i * order + j] = -Y[i * order + j];
		}
	}
	return Y;
}

template<typename U>
Matrix<U> Matrix<U>::minor(int row, int col)
{
	Matrix<U> dest(Rows - 1, Columns - 1);
	int colCount = 0, rowCount = 0;
	for (int i = 0; i < Rows; i++)
	{
		if (i != row)
		{
			colCount = 0;
			for (int j = 0; j < Columns; j++)
			{
				if (j != col)
				{
					dest[rowCount*(Columns-1) + colCount] = matrix[i*Columns + j];
					colCount++;
				}
			}
			rowCount++;
		}
	}
	return dest;
}

template<typename U>
U Matrix<U>::det()
{
	if (Rows == 1)
		return matrix[0];
	U res = 0;
	for (int i = 0; i < Rows; i++)
	{
		Matrix<U> cur_minor = minor(0, i);

		res += (i % 2 ? -1.0 : 1.0) * matrix[i] * cur_minor.det();
		//for better porfomance: (i % 2 ? -1.0 : 1.0) ==  pow( -1.0, i )
	}
	return res;
}

template<typename U>
U Matrix<U>::norm()
{
	U max = 0;
	U sum = 0;
	for (int j = 0; j < Columns; j++)
	{
		for (int i = 0; i < Rows; i++)
			sum += abs(matrix[i * Columns + j]);
		if (max < sum) max = sum;
		sum = 0;
	}
	return max;
}

template<typename U>
U Matrix<U>::cond() {
	return norm() * norm(inverse());
}

#endif