#ifndef XGL_View_INL
#define XGL_View_INL

#include "View.h"

namespace XGL
{
	template<typename T>
	Matrix<T, 4, 4, false> View::lookAt(Vector<T, 3> pos, Vector<T, 3> target, Vector<T, 3> up)
	{
		Matrix<T, 4, 4, false> res;
		Vector<T, 3> z = (pos - target).normalize();
		Vector<T, 3> x = Vector<T, 3>::cross(up, z).normalize();
		Vector<T, 3> y = Vector<T, 3>::cross(z, x);

		res(0, 0) = x.x();
		res(0, 1) = x.y();
		res(0, 2) = x.z();
		res(0, 3) = -Vector<T, 3>::dot(pos, x);
		res(1, 0) = y.x();
		res(1, 1) = y.y();
		res(1, 2) = y.z();
		res(1, 3) = -Vector<T, 3>::dot(pos, y);
		res(2, 0) = z.x();
		res(2, 1) = z.y();
		res(2, 2) = z.z();
		res(2, 3) = -Vector<T, 3>::dot(pos, z);
		res(3, 3) = 1;

		return res;
	}

	template<typename T>
	Matrix<T, 4, 4, false> View::euler(Vector<T, 3> pos, T yaw, T pitch, T roll)
	{
		Matrix<T, 4, 4, false> res;
		T s1 = sin(-yaw), s2 = sin(pitch), s3 = sin(-roll);
		T c1 = cos(-yaw), c2 = cos(pitch), c3 = cos(-roll);

		res(0, 0) = c1 * c3 + s1 * s2 * s3;
		res(0, 1) = c2 * s3;
		res(0, 2) = c1 * s2 * s3 - c3 * s1;
		res(0, 3) = res(0, 0) * -pos.x() + res(0, 1) * -pos.y() + res(0, 2) * -pos.z();
		res(1, 0) = c3 * s1 * s2 - c1 * s3;
		res(1, 1) = c2 * c3;
		res(1, 2) = c1 * c3 * s2 + s1 * s3;
		res(1, 3) = res(1, 0) * -pos.x() + res(1, 1) * -pos.y() + res(1, 2) * -pos.z();
		res(2, 0) = c2 * s1;
		res(2, 1) = -s2;
		res(2, 2) = c1 * c2;
		res(2, 3) = res(2, 0) * -pos.x() + res(2, 1) * -pos.y() + res(2, 2) * -pos.z();
		res(3, 3) = 1;
		return res;
	}
}

#endif // !XGL_View_INL
