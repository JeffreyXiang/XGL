#ifndef XGL_VECTOR_H
#define XGL_VECTOR_H

#include <iostream>

namespace XGL
{
	template<typename T, int size>
	class Vector;

	template<typename T, int size>
	class IVector
	{
		public:
			enum ERROR { INVALID_SIZE, OUT_OF_RANGE, DIVISION_BY_ZERO, ZERO_VECTOR };

		protected:
			T* data;

		public:
			IVector();
			IVector(const IVector<T, size>& other);
			IVector(IVector<T, size>&& other);
			~IVector() { delete[] data; }

			Vector<T, size>& operator=(const IVector<T, size>& rOpnt);
			Vector<T, size>& operator=(IVector<T, size>&& rOpnt);

			T& operator[](size_t idx) const;
			T& operator()(size_t idx) const;

			Vector<T, size> operator-() const;

			Vector<T, size> operator+(const Vector<T, size>& rOpnt) const;
			Vector<T, size>& operator+=(const Vector<T, size>& rOpnt);

			Vector<T, size> operator-(const Vector<T, size>& rOpnt) const;
			Vector<T, size>& operator-=(const Vector<T, size>& rOpnt);

			Vector<T, size> operator*(const Vector<T, size>& rOpnt) const;
			Vector<T, size>& operator*=(const Vector<T, size>& rOpnt);

			Vector<T, size> operator/(const Vector<T, size>& rOpnt) const;
			Vector<T, size>& operator/=(const Vector<T, size>& rOpnt);

			Vector<T, size> operator+(T rOpnt) const;
			template<typename T, int size>
			friend Vector<T, size> operator+(T lOpnt, const Vector<T, size>& rOpnt);
			Vector<T, size>& operator+=(T rOpnt);

			Vector<T, size> operator-(T rOpnt) const;
			template<typename T, int size>
			friend Vector<T, size> operator-(T lOpnt, const Vector<T, size>& rOpnt);
			Vector<T, size>& operator-=(T rOpnt);

			Vector<T, size> operator*(T rOpnt) const;
			template<typename T, int size>
			friend Vector<T, size> operator*(T lOpnt, const Vector<T, size>& rOpnt);
			Vector<T, size>& operator*=(T rOpnt);

			Vector<T, size> operator/(T rOpnt) const;
			template<typename T, int size>
			friend Vector<T, size> operator/(T lOpnt, const Vector<T, size>& rOpnt);
			Vector<T, size>& operator/=(T rOpnt);

			bool operator==(const Vector<T, size>& rOpnt) const;
			bool operator!=(const Vector<T, size>& rOpnt) const;

			static T dot(const Vector<T, size>& lOpnt, const Vector<T, size>& rOpnt);
			T dot(const Vector<T, size>& rOpnt) const;

			static T norm2(const Vector<T, size>& Opnt);
			T norm2() const;

			static T norm(const Vector<T, size>& Opnt);
			T norm() const;

			static Vector<T, size> normalize(const Vector<T, size>& Opnt);
			Vector<T, size> normalize() const;

			Vector<T, size>& fill(T elem);

			T* getData() const { return data; }

			template<typename T, int size>
			friend std::ostream& operator<<(std::ostream& output, const Vector<T, size>& rOpnt);
	};

	template<typename T, int size>
	class Vector : public IVector<T, size>
	{
		public:
			using IVector<T, size>::IVector;
	};

	template<typename T>
	class Vector<T, 2> : public IVector<T, 2>
	{
		public:
			using IVector<T, 2>::IVector;
			Vector(T x, T y) { this->data[0] = x; this->data[1] = y; }

			T& x() const { return this->data[0]; }
			T& y() const { return this->data[1]; }
	};

	template<typename T>
	class Vector<T, 3> : public IVector<T, 3>
	{
		public:
			using IVector<T, 3>::IVector;
			Vector(T x, T y, T z) { this->data[0] = x; this->data[1] = y; this->data[2] = z; }
			Vector(Vector<T, 2> xy, T z) { this->data[0] = xy[0]; this->data[1] = xy[1]; this->data[2] = z; }
			Vector(T x, Vector<T, 2> yz) { this->data[0] = x; this->data[1] = yz[0]; this->data[2] = yz[1]; }

			static Vector<T, 3> cross(const Vector<T, 3>& lOpnt, const Vector<T, 3>& rOpnt);
			Vector<T, 3> cross(const Vector<T, 3>& rOpnt) const;

			T& x() const { return this->data[0]; }
			T& y() const { return this->data[1]; }
			T& z() const { return this->data[2]; }
	};

	template<typename T>
	class Vector<T, 4> : public IVector<T, 4>
	{
		public:
			using IVector<T, 4>::IVector;
			Vector(T x, T y, T z, T w) { this->data[0] = x; this->data[1] = y; this->data[2] = z; this->data[3] = w; }
			Vector(Vector<T, 3> xyz, T w) { this->data[0] = xyz[0]; this->data[1] = xyz[1]; this->data[2] = xyz[2]; this->data[3] = w; }
			Vector(T x, Vector<T, 3> yzw) { this->data[0] = x; this->data[1] = yzw[0]; this->data[2] = yzw[1]; this->data[3] = yzw[2]; }
			Vector(Vector<T, 2> xy, T z, T w) { this->data[0] = xy[0]; this->data[1] = xy[1]; this->data[2] = z; this->data[3] = w; }
			Vector(T x, Vector<T, 2> yz, T w) { this->data[0] = x; this->data[1] = yz[0]; this->data[2] = yz[1]; this->data[3] = w; }
			Vector(T x, T y, Vector<T, 2> zw) { this->data[0] = x; this->data[1] = y; this->data[2] = zw.ta[0]; this->data[3] = zw[1]; }

			T& x() const { return this->data[0]; }
			T& y() const { return this->data[1]; }
			T& z() const { return this->data[2]; }
			T& w() const { return this->data[3]; }
	};

	using Vec2 = Vector<float, 2>;
	using Vec3 = Vector<float, 3>;
	using Vec4 = Vector<float, 4>;
}

#include "Vector.inl"

#endif // !XGL_VECTOR_H
