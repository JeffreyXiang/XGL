#ifndef XGL_Transform_INL
#define XGL_Transform_INL

#include "Transform.h"

namespace XGL
{
	template<typename T>
	Matrix<T, 4, 4, false> Transform::scale(T k)
	{
		Matrix<T, 4, 4, false> res;
		res(0, 0) = res(1, 1) = res(2, 2) = k;
		res(3, 3) = 1;
		return res;
	}

	template<typename T>
	Matrix<T, 4, 4, false>& Transform::scale(Matrix<T, 4, 4, false>& source, T k)
	{
		for (size_t i = 0; i < 3; i++)
			for (size_t j = 0; j < 4; j++)
				source(i, j) *= k;
		return source;
	}

	template<typename T>
	Matrix<T, 4, 4, false> Transform::scale(T kx, T ky, T kz)
	{
		Matrix<T, 4, 4, false> res;
		res(0, 0) = kx;
		res(1, 1) = ky;
		res(2, 2) = kz;
		res(3, 3) = 1;
		return res;
	}

	template<typename T>
	Matrix<T, 4, 4, false>& Transform::scale(Matrix<T, 4, 4, false>& source, T kx, T ky, T kz)
	{
		for (size_t i = 0; i < 4; i++)
			source(0, i) *= kx;
		for (size_t i = 0; i < 4; i++)
			source(1, i) *= ky;
		for (size_t i = 0; i < 4; i++)
			source(2, i) *= kz;
		return source;
	}

	template<typename T>
	Matrix<T, 4, 4, false> Transform::rotate(T yaw, T pitch, T roll)
	{
		Matrix<T, 4, 4, false> res;
		T s1 = sin(-yaw), s2 = sin(pitch), s3 = sin(-roll);
		T c1 = cos(-yaw), c2 = cos(pitch), c3 = cos(-roll);

		res(0, 0) = c1 * c3 + s1 * s2 * s3;
		res(0, 1) = c3 * s1 * s2 - c1 * s3;
		res(0, 2) = c2 * s1;
		res(1, 0) = c2 * s3;
		res(1, 1) = c2 * c3;
		res(1, 2) = -s2;
		res(2, 0) = c1 * s2 * s3 - c3 * s1;
		res(2, 1) = c1 * c3 * s2 + s1 * s3;
		res(2, 2) = c1 * c2;
		res(3, 3) = 1;

		return res;
	}

	template<typename T>
	Matrix<T, 4, 4, false>& Transform::rotate(Matrix<T, 4, 4, false>& source, T yaw, T pitch, T roll)
	{
		source = rotate(yaw, pitch, roll) * source;
		return source;
	}

	template<typename T>
	Matrix<T, 4, 4, false> Transform::rotate(T angle, Vector<T, 3> axis)
	{
		axis = axis.normalize();
		Matrix<T, 4, 4, false> res;
		T s = sin(angle), c = cos(angle);

		res(0, 0) = c + axis.x() * axis.x() * (1 - c);
		res(0, 1) = axis.x() * axis.y() * (1 - c) - axis.z() * s;
		res(0, 2) = axis.x() * axis.z() * (1 - c) + axis.y() * s;
		res(1, 0) = axis.y() * axis.x() * (1 - c) + axis.z() * s;
		res(1, 1) = c + axis.y() * axis.y() * (1 - c);
		res(1, 2) = axis.y() * axis.z() * (1 - c) - axis.x() * s;
		res(2, 0) = axis.z() * axis.x() * (1 - c) - axis.y() * s;
		res(2, 1) = axis.z() * axis.y() * (1 - c) + axis.x() * s;
		res(2, 2) = c + axis.z() * axis.z() * (1 - c);
		res(3, 3) = 1;

		return res;
	}

	template<typename T>
	Matrix<T, 4, 4, false>& Transform::rotate(Matrix<T, 4, 4, false>& source, T angle, Vector<T, 3> axis)
	{
		source = rotate(angle, axis) * source;
		return source;
	}

	template<typename T>
	Matrix<T, 4, 4, false> Transform::translate(Vector<T, 3> shift)
	{
		Matrix<T, 4, 4, false> res = Matrix<T, 4, 4, false>::identity();
		res(0, 3) = shift.x();
		res(1, 3) = shift.y();
		res(2, 3) = shift.z();
		return res;
	}

	template<typename T>
	Matrix<T, 4, 4, false>& Transform::translate(Matrix<T, 4, 4, false>& source, Vector<T, 3> shift)
	{
		source = translate(shift) * source;
		return source;
	}
}

#endif // !XGL_Transform_INL
