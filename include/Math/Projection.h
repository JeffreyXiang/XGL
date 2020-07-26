#ifndef XGL_PROJECTION_H
#define XGL_PROJECTION_H

#include "Vector.h"
#include "Matrix.h"

namespace XGL
{
	class Projection
	{
		public:
			enum ERROR { INVALID_SIZE };

		public:
			template<typename T>
			static Matrix<T, 4, 4, false> orthogonal(T left, T right, T bottom, T top, T near, T far);
			template<typename T>
			static Matrix<T, 4, 4, false> orthoSym(T width, T height, T near, T far);
			template<typename T>
			static Matrix<T, 4, 4, false> orthoAR(T height, T aspect, T near, T far);
			template<typename T>
			static Matrix<T, 4, 4, false> perspective(T left, T right, T bottom, T top, T near, T far);
			template<typename T>
			static Matrix<T, 4, 4, false> perspSym(T width, T height, T near, T far);
			template<typename T>
			static Matrix<T, 4, 4, false> perspAR(T height, T aspect, T near, T far);
			template<typename T>
			static Matrix<T, 4, 4, false> perspFov(T fovy, T aspect, T near, T far);
	};
}

#include "Projection.inl"

#endif // !XGL_PROJECTION_H
