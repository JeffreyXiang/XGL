#ifndef XGL_VIEW_H
#define XGL_VIEW_H

#include "Vector.h"
#include "Matrix.h"

namespace XGL
{
	class View
	{
		public:
			enum ERROR {};

		public:
			template<typename T>
			static Matrix<T, 4, 4, false> lookAt(Vector<T, 3> pos, Vector<T, 3> target, Vector<T, 3> up);
			template<typename T>
			static Matrix<T, 4, 4, false> euler(Vector<T, 3> pos, T yaw, T pitch, T roll);
	};
}

#include "View.inl"

#endif // !XGL_VIEW_H
