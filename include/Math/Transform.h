#ifndef XGL_TRANSFORM_H
#define XGL_TRANSFORM_H

#include "Vector.h"
#include "Matrix.h"

namespace XGL
{
	class Transform
	{
		public:
			enum ERROR {};

		public:
			template<typename T>
			static Matrix<T, 4, 4, false> scale(T k);
			template<typename T>
			static Matrix<T, 4, 4, false>& scale(Matrix<T, 4, 4, false>& source, T k);
			template<typename T>
			static Matrix<T, 4, 4, false> scale(T kx, T ky, T kz);
			template<typename T>
			static Matrix<T, 4, 4, false>& scale(Matrix<T, 4, 4, false>& source, T kx, T ky, T kz);

			template<typename T>
			static Matrix<T, 4, 4, false> rotate(T yaw, T pitch, T roll);
			template<typename T>
			static Matrix<T, 4, 4, false>& rotate(Matrix<T, 4, 4, false>& source, T yaw, T pitch, T roll);
			template<typename T>
			static Matrix<T, 4, 4, false> rotate(T angle, Vector<T, 3> axis);
			template<typename T>
			static Matrix<T, 4, 4, false>& rotate(Matrix<T, 4, 4, false>& source, T angle, Vector<T, 3> axis);

			template<typename T>
			static Matrix<T, 4, 4, false> translate(Vector<T, 3> shift);
			template<typename T>
			static Matrix<T, 4, 4, false>& translate(Matrix<T, 4, 4, false>& source, Vector<T, 3> shift);
	};
}

#include "Transform.inl"

#endif // !XGL_TRANSFORM_H
