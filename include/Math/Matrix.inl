#ifndef XGL_MATRIX_INL
#define XGL_MATRIX_INL

#include "Matrix.h"
#include <iomanip>

namespace XGL
{
	template<typename T, int rows, int columns, bool major>
	IMatrix<T, rows, columns, major>::IMatrix()
	{
		if (rows <= 0 || columns <= 0)
		{
			std::cerr << "ERROR | XGL::IMatrix::IMatrix() : Invalid size.\n";
			throw INVALID_SIZE;
		}
		data = new T[rows * columns];
		memset(data, 0, rows * columns * sizeof(T));
	}

	template<typename T, int rows, int columns, bool major>
	IMatrix<T, rows, columns, major>::IMatrix(const IMatrix<T, rows, columns, major>& other)
	{
		data = new T[rows * columns];
		memcpy(data, other.data, rows * columns * sizeof(T));
	}

	template<typename T, int rows, int columns, bool major>
	IMatrix<T, rows, columns, major>::IMatrix(IMatrix<T, rows, columns, major>&& other)
	{
		data = other.data;
		other.data = NULL;
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major>& IMatrix<T, rows, columns, major>::operator=(const IMatrix<T, rows, columns, major>& rOpnt)
	{
		if (data != rOpnt.data)
		{
			delete[] data;
			data = new T[rows * columns];
			memcpy(data, rOpnt.data, rows * columns * sizeof(T));
		}
		return *static_cast<Matrix<T, rows, columns, major>*>(this);
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major>& IMatrix<T, rows, columns, major>::operator=(IMatrix<T, rows, columns, major>&& rOpnt)
	{
		delete[] data;
		data = rOpnt.data;
		rOpnt.data = NULL;
		return *static_cast<Matrix<T, rows, columns, major>*>(this);
	}

	template<typename T, int rows, int columns, bool major>
	T& IMatrix<T, rows, columns, major>::operator()(size_t rowIdx, size_t colIdx) const
	{
		if (rowIdx < 0 || rowIdx >= rows || colIdx < 0 || colIdx >= columns)
		{
			std::cerr << "ERROR | XGL::IMatrix::operator()(size_t, size_t) : Index out of bounds.\n";
			throw OUT_OF_RANGE;
		}
		if (major)
			return data[rowIdx * columns + colIdx];
		else
			return data[colIdx * rows + rowIdx];
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major> IMatrix<T, rows, columns, major>::operator-()
	{
		Matrix<T, rows, columns, major> res;
		for (size_t i = 0; i < rows * columns; i++)
		{
			res.data[i] = -data[i];
		}
		return res;
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major> IMatrix<T, rows, columns, major>::operator+(const Matrix<T, rows, columns, major>& rOpnt)
	{
		Matrix<T, rows, columns, major> res;
		for (size_t i = 0; i < rows * columns; i++)
		{
			res.data[i] = data[i] + rOpnt.data[i];
		}
		return res;
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major>& IMatrix<T, rows, columns, major>::operator+=(const Matrix<T, rows, columns, major>& rOpnt)
	{
		for (size_t i = 0; i < rows * columns; i++)
		{
			data[i] += rOpnt.data[i];
		}
		return *static_cast<Matrix<T, rows, columns, major>*>(this);
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major> IMatrix<T, rows, columns, major>::operator-(const Matrix<T, rows, columns, major>& rOpnt)
	{
		Matrix<T, rows, columns, major> res;
		for (size_t i = 0; i < rows * columns; i++)
		{
			res.data[i] = data[i] - rOpnt.data[i];
		}
		return res;
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major>& IMatrix<T, rows, columns, major>::operator-=(const Matrix<T, rows, columns, major>& rOpnt)
	{
		for (size_t i = 0; i < rows * columns; i++)
		{
			data[i] -= rOpnt.data[i];
		}
		return *static_cast<Matrix<T, rows, columns, major>*>(this);
	}

	template<typename T, int rows, int columns, bool major>
	template<int rOpntColumns>
	Matrix<T, rows, rOpntColumns, major> IMatrix<T, rows, columns, major>::operator*(const Matrix<T, columns, rOpntColumns, major>& rOpnt)
	{
		Matrix<T, rows, rOpntColumns, major> res;
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < rOpntColumns; j++)
			{
				for (size_t k = 0; k < columns; k++)
				{
					res(i, j) += (*this)(i, k) * rOpnt(k, j);
				}
			}
		}
		return res;
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major> IMatrix<T, rows, columns, major>::operator+(T rOpnt)
	{
		Matrix<T, rows, columns, major> res;
		for (size_t i = 0; i < rows * columns; i++)
			res.data[i] = data[i] + rOpnt;
		return res;
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major> operator+(T lOpnt, const Matrix<T, rows, columns, major>& rOpnt)
	{
		Matrix<T, rows, columns, major> res;
		for (size_t i = 0; i < rows * columns; i++)
			res.data[i] = lOpnt + rOpnt.data[i];
		return res;
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major>& IMatrix<T, rows, columns, major>::operator+=(T rOpnt)
	{
		for (size_t i = 0; i < rows * columns; i++)
			data[i] += rOpnt;
		return *static_cast<Matrix<T, rows, columns, major>*>(this);
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major> IMatrix<T, rows, columns, major>::operator-(T rOpnt)
	{
		Matrix<T, rows, columns, major> res;
		for (size_t i = 0; i < rows * columns; i++)
			res.data[i] = data[i] - rOpnt;
		return res;
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major> operator-(T lOpnt, const Matrix<T, rows, columns, major>& rOpnt)
	{
		Matrix<T, rows, columns, major> res;
		for (size_t i = 0; i < rows * columns; i++)
			res.data[i] = lOpnt - rOpnt.data[i];
		return res;
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major>& IMatrix<T, rows, columns, major>::operator-=(T rOpnt)
	{
		for (size_t i = 0; i < rows * columns; i++)
			data[i] -= rOpnt;
		return *static_cast<Matrix<T, rows, columns, major>*>(this);
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major> IMatrix<T, rows, columns, major>::operator*(T rOpnt)
	{
		Matrix<T, rows, columns, major> res;
		for (size_t i = 0; i < rows * columns; i++)
			res.data[i] = data[i] * rOpnt;
		return res;
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major> operator*(T lOpnt, const Matrix<T, rows, columns, major>& rOpnt)
	{
		Matrix<T, rows, columns, major> res;
		for (size_t i = 0; i < rows * columns; i++)
			res.data[i] = lOpnt * rOpnt.data[i];
		return res;
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major>& IMatrix<T, rows, columns, major>::operator*=(T rOpnt)
	{
		for (size_t i = 0; i < rows * columns; i++)
			data[i] *= rOpnt;
		return *static_cast<Matrix<T, rows, columns, major>*>(this);
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major> IMatrix<T, rows, columns, major>::operator/(T rOpnt)
	{
		if (rOpnt == 0)
		{
			std::cerr << "ERROR | XGL::IMatrix::operator/(T) : Division by zero.\n";
			throw DIVISION_BY_ZERO;
		}
		Matrix<T, rows, columns, major> res;
		for (size_t i = 0; i < rows * columns; i++)
			res.data[i] = data[i] / rOpnt;
		return res;
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major> operator/(T lOpnt, const Matrix<T, rows, columns, major>& rOpnt)
	{
		Matrix<T, rows, columns, major> res;
		for (size_t i = 0; i < rows * columns; i++)
		{
			if (rOpnt.data[i] == 0)
			{
				std::cerr << "ERROR | XGL::operator/(T, const Matrix<T, rows, columns, major>&) : Division by zero.\n";
				throw IMatrix<T, rows, columns, major>::DIVISION_BY_ZERO;
			}
			res.data[i] = lOpnt / rOpnt.data[i];
		}
		return res;
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, major>& IMatrix<T, rows, columns, major>::operator/=(T rOpnt)
	{
		if (rOpnt == 0)
		{
			std::cerr << "ERROR | XGL::IMatrix::operator/=(T) : Division by zero.\n";
			throw DIVISION_BY_ZERO;
		}
		for (size_t i = 0; i < rows * columns; i++)
			data[i] /= rOpnt;
		return *static_cast<Matrix<T, rows, columns, major>*>(this);
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, columns, rows, major> IMatrix<T, rows, columns, major>::transpose(const Matrix<T, rows, columns, major>& Opnt)
	{
		Matrix<T, columns, rows, major> res;
		for (size_t i = 0; i < rows; i++)
			for (size_t j = 0; j < columns; j++)
				res(j, i) = Opnt(i, j);
		return res;
	}

	template<typename T, int rows, int columns, bool major>
	Matrix<T, columns, rows, major> IMatrix<T, rows, columns, major>::transpose()
	{
		Matrix<T, columns, rows, major> res;
		for (size_t i = 0; i < rows; i++)
			for (size_t j = 0; j < columns; j++)
				res(j, i) = (*this)(i, j);
		return res;
	}

	template<typename T, int size, bool major>
	Matrix<T, size, size, major>& Matrix<T, size, size, major>::operator*=(const Matrix<T, size, size, major>& rOpnt)
	{
		Matrix<T, size, size, major> res;
		for (size_t i = 0; i < size; i++)
		{
			for (size_t j = 0; j < size; j++)
			{
				for (size_t k = 0; k < size; k++)
				{
					res(i, j) += (*this)(i, k) * rOpnt(k, j);
				}
			}
		}
		delete[] this->data;
		this->data = res.data;
		res.data = NULL;
		return *static_cast<Matrix<T, size, size, major>*>(this);
	}

	template<typename T, int size, bool major>
	Matrix<T, size, size, major> Matrix<T, size, size, major>::identity()
	{
		Matrix<T, size, size, major> res;
		for (size_t i = 0; i < size; i++)
			res(i, i) = 1;
		return res;
	}

	template<bool tarMajor, typename T, int rows, int columns, bool major>
	Matrix<T, rows, columns, tarMajor> matrix_major_cast(Matrix<T, rows, columns, major>& src)
	{
		Matrix<T, rows, columns, tarMajor> res;
		for (size_t i = 0; i < rows; i++)
			for (size_t j = 0; j < columns; j++)
				res(i, j) = src(i, j);
		return res;
	}

	template<typename T, int rows, int columns, bool major>
	std::ostream& operator<<(std::ostream& output, const Matrix<T, rows, columns, major>& rOpnt)
	{
		output.setf(std::ios_base::fixed);
		output.precision(2);
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				output << std::setw(5) << rOpnt(i, j) << ' ';
			}
			output << std::endl;
		}
		output.unsetf(std::ios_base::fixed);
		output.precision(6);
		return output;
	}
}

#endif // !XGL_MATRIX_H
