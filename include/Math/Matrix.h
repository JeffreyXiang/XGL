#ifndef XGL_MATRIX_H
#define XGL_MATRIX_H

#include <iostream>

namespace XGL
{
	template<typename T, int rows, int columns, bool major>
	class Matrix;

	template<typename T, int rows, int columns, bool major>
	class IMatrix
	{
		public:
			enum ERROR { INVALID_SIZE, OUT_OF_RANGE, DIVISION_BY_ZERO, ZERO_VECTOR };

		protected:
			T* data;

		public:
			IMatrix();
			IMatrix(const IMatrix<T, rows, columns, major>& other);
			IMatrix(IMatrix<T, rows, columns, major>&& other);
			~IMatrix() { delete[] data; }

			Matrix<T, rows, columns, major>& operator=(const IMatrix<T, rows, columns, major>& rOpnt);
			Matrix<T, rows, columns, major>& operator=(IMatrix<T, rows, columns, major>&& rOpnt);

			T& operator()(size_t rowIdx, size_t colIdx) const;

			Matrix<T, rows, columns, major> operator-();

			Matrix<T, rows, columns, major> operator+(const Matrix<T, rows, columns, major>& rOpnt);
			Matrix<T, rows, columns, major>& operator+=(const Matrix<T, rows, columns, major>& rOpnt);

			Matrix<T, rows, columns, major> operator-(const Matrix<T, rows, columns, major>& rOpnt);
			Matrix<T, rows, columns, major>& operator-=(const Matrix<T, rows, columns, major>& rOpnt);

			template<int rOpntColumns>
			Matrix<T, rows, rOpntColumns, major> operator*(const Matrix<T, columns, rOpntColumns, major>& rOpnt);

			template<bool tarMajor, typename T, int rows, int columns, bool major>
			friend Matrix<T, rows, columns, tarMajor> matrix_major_cast(Matrix<T, rows, columns, major>& src);

			Matrix<T, rows, columns, major> operator+(T rOpnt);
			template<typename T, int rows, int columns, bool major>
			friend Matrix<T, rows, columns, major> operator+(T lOpnt, const Matrix<T, rows, columns, major>& rOpnt);
			Matrix<T, rows, columns, major>& operator+=(T rOpnt);

			Matrix<T, rows, columns, major> operator-(T rOpnt);
			template<typename T, int rows, int columns, bool major>
			friend Matrix<T, rows, columns, major> operator-(T lOpnt, const Matrix<T, rows, columns, major>& rOpnt);
			Matrix<T, rows, columns, major>& operator-=(T rOpnt);

			Matrix<T, rows, columns, major> operator*(T rOpnt);
			template<typename T, int rows, int columns, bool major>
			friend Matrix<T, rows, columns, major> operator*(T lOpnt, const Matrix<T, rows, columns, major>& rOpnt);
			Matrix<T, rows, columns, major>& operator*=(T rOpnt);

			Matrix<T, rows, columns, major> operator/(T rOpnt);
			template<typename T, int rows, int columns, bool major>
			friend Matrix<T, rows, columns, major> operator/(T lOpnt, const Matrix<T, rows, columns, major>& rOpnt);
			Matrix<T, rows, columns, major>& operator/=(T rOpnt);

			static Matrix<T, columns, rows, major> transpose(const Matrix<T, rows, columns, major>& Opnt);
			Matrix<T, columns, rows, major> transpose();

			T* rawData() { return data; }

			template<typename T, int rows, int columns, bool major>
			friend std::ostream& operator<<(std::ostream& output, const Matrix<T, rows, columns, major>& rOpnt);
	};

	template<typename T, int rows, int columns, bool major>
	class Matrix : public IMatrix<T, rows, columns, major>
	{
	public:
		using IMatrix<T, rows, columns, major>::IMatrix;
	};

	template<typename T, int size, bool major>
	class Matrix<T, size, size, major> : public IMatrix<T, size, size, major>
	{
		public:
			using IMatrix<T, size, size, major>::IMatrix;

			Matrix<T, size, size, major>& operator*=(const Matrix<T, size, size, major>& rOpnt);

			static Matrix<T, size, size, major> identity();
	};

	using Mat2 = Matrix<float, 2, 2, false>;
	using Mat3 = Matrix<float, 3, 3, false>;
	using Mat4 = Matrix<float, 4, 4, false>;
}

#include "Matrix.inl"

#endif // !XGL_MATRIX_H
