#ifndef XGL_PROJECTION_INL
#define XGL_PROJECTION_INL

#include "Projection.h"
#include <iostream>

namespace XGL
{
	template<typename T>
	Matrix<T, 4, 4, false> Projection::orthogonal(T left, T right, T bottom, T top, T near, T far)
	{
		if (right <= left || top <= bottom || near <= 0 || far <= near)
		{
			std::cerr << "ERROR | XGL::Projection::orthogonal(T, T, T, T, T, T) : Invalid size.\n";
			throw INVALID_SIZE;
		}
		Matrix<T, 4, 4, false> res;
		res(0, 0) = 2 / (right - left);
		res(0, 3) = -(right + left) / (right - left);
		res(1, 1) = 2 / (top - bottom);
		res(1, 3) = -(top + bottom) / (top - bottom);
		res(2, 2) = -2 / (far - near);
		res(2, 3) = -(far + near) / (far - near);
		res(3, 3) = 1;
		return res;
	}

	template<typename T>
	Matrix<T, 4, 4, false> Projection::orthoSym(T width, T height, T near, T far)
	{
		if (width <= 0 || height <= 0 || near <= 0 || far <= near)
		{
			std::cerr << "ERROR | XGL::Projection::orthoSym(T, T, T, T) : Invalid size.\n";
			throw INVALID_SIZE;
		}
		Matrix<T, 4, 4, false> res;
		res(0, 0) = 2 / width;
		res(1, 1) = 2 / height;
		res(2, 2) = -2 / (far - near);
		res(2, 3) = -(far + near) / (far - near);
		res(3, 3) = 1;
		return res;
	}

	template<typename T>
	Matrix<T, 4, 4, false> Projection::orthoAR(T height, T aspect, T near, T far)
	{
		if (height <= 0 || aspect <= 0 || near <= 0 || far <= near)
		{
			std::cerr << "ERROR | XGL::Projection::orthoAR(T, T, T, T) : Invalid size.\n";
			throw INVALID_SIZE;
		}
		return orthoSym(height * aspect, height, near, far);
	}

	template<typename T>
	Matrix<T, 4, 4, false> Projection::perspective(T left, T right, T bottom, T top, T near, T far)
	{
		if (right <= left || top <= bottom || near <= 0 || far <= near)
		{
			std::cerr << "ERROR | XGL::Projection::perspective(T, T, T, T, T, T) : Invalid size.\n";
			throw INVALID_SIZE;
		}
		Matrix<T, 4, 4, false> res;
		res(0, 0) = 2 * near / (right - left);
		res(0, 2) = (right + left) / (right - left);
		res(1, 1) = 2 * near / (top - bottom);
		res(1, 2) = (top + bottom) / (top - bottom);
		res(2, 2) = -(far + near) / (far - near);
		res(2, 3) = -2 * far * near / (far - near);
		res(3, 2) = -1;
		return res;
	}

	template<typename T>
	Matrix<T, 4, 4, false> Projection::perspSym(T width, T height, T near, T far)
	{
		if (width <= 0 || height <= 0 || near <= 0 || far <= near)
		{
			std::cerr << "ERROR | XGL::Projection::perspSym(T, T, T, T) : Invalid size.\n";
			throw INVALID_SIZE;
		}
		Matrix<T, 4, 4, false> res;
		res(0, 0) = 2 * near / width;
		res(1, 1) = 2 * near / height;
		res(2, 2) = -(far + near) / (far - near);
		res(2, 3) = -2 * far * near / (far - near);
		res(3, 2) = -1;
		return res;
	}

	template<typename T>
	Matrix<T, 4, 4, false> Projection::perspAR(T height, T aspect, T near, T far)
	{
		if (height <= 0 || aspect <= 0 || near <= 0 || far <= near)
		{
			std::cerr << "ERROR | XGL::Projection::perspAR(T, T, T, T) : Invalid size.\n";
			throw INVALID_SIZE;
		}
		return perspSym(height * aspect, height, near, far);
	}

	template<typename T>
	Matrix<T, 4, 4, false> Projection::perspFov(T fovy, T aspect, T near, T far)
	{
		if (fovy <= 0 || aspect <= 0 || near <= 0 || far <= near)
		{
			std::cerr << "ERROR | XGL::Projection::perspFov(T, T, T, T) : Invalid size.\n";
			throw INVALID_SIZE;
		}
		return perspAR(2 * near * tan(fovy / 2), aspect, near, far);
	}
}

#endif // !XGL_PROJECTION_INL
