#ifndef XGL_VECTOR_INL
#define XGL_VECTOR_INL

#include "Vector.h"
#include <cstring>

namespace XGL
{
	template<typename T, int size>
	IVector<T, size>::IVector()
	{
		if (size <= 0)
		{
			std::cerr << "ERROR | XGL::IVector::IVector() : Invalid size.\n";
			throw INVALID_SIZE;
		}
		data = new T[size];
		memset(data, 0, size * sizeof(T));
	}

	template<typename T, int size>
	IVector<T, size>::IVector(const IVector<T, size>& other)
	{
		data = new T[size];
		memcpy(data, other.data, size * sizeof(T));
	}

	template<typename T, int size>
	IVector<T, size>::IVector(IVector<T, size>&& other)
	{
		data = other.data;
		other.data = NULL;
	}

	template<typename T, int size>
	Vector<T, size>& IVector<T, size>::operator=(const IVector<T, size>& rOpnt)
	{
		if (data != rOpnt.data)
		{
			delete[] data;
			data = new T[size];
			memcpy(data, rOpnt.data, size * sizeof(T));
		}
		return *static_cast<Vector<T, size>*>(this);
	}

	template<typename T, int size>
	Vector<T, size>& IVector<T, size>::operator=(IVector<T, size>&& rOpnt)
	{
		delete[] data;
		data = rOpnt.data;
		rOpnt.data = NULL;
		return *static_cast<Vector<T, size>*>(this);
	}

	template<typename T, int size>
	T& IVector<T, size>::operator[](size_t idx) const
	{
		if (idx < 0 || idx >= size)
		{
			std::cerr << "ERROR | XGL::IVector::operator[](size_t) : Index out of bounds.\n";
			throw OUT_OF_RANGE;
		}
		return data[idx];
	}

	template<typename T, int size>
	T& IVector<T, size>::operator()(size_t idx) const
	{
		if (idx < 0 || idx >= size)
		{
			std::cerr << "ERROR | XGL::IVector::operator()(size_t) : Index out of bounds.\n";
			throw OUT_OF_RANGE;
		}
		return data[idx];
	}

	template<typename T, int size>
	Vector<T, size> IVector<T, size>::operator-() const
	{
		Vector<T, size> res;
		for (size_t i = 0; i < size; i++)
			res.data[i] = -data[i];
		return res;
	}

	template<typename T, int size>
	Vector<T, size> IVector<T, size>::operator+(const Vector<T, size>& rOpnt) const
	{
		Vector<T, size> res;
		for (size_t i = 0; i < size; i++)
			res.data[i] = data[i] + rOpnt.data[i];
		return res;
	}

	template<typename T, int size>
	Vector<T, size>& IVector<T, size>::operator+=(const Vector<T, size>& rOpnt)
	{
		for (size_t i = 0; i < size; i++)
			data[i] += rOpnt.data[i];
		return *static_cast<Vector<T, size>*>(this);
	}

	template<typename T, int size>
	Vector<T, size> IVector<T, size>::operator-(const Vector<T, size>& rOpnt) const
	{
		Vector<T, size> res;
		for (size_t i = 0; i < size; i++)
			res.data[i] = data[i] - rOpnt.data[i];
		return res;
	}

	template<typename T, int size>
	Vector<T, size>& IVector<T, size>::operator-=(const Vector<T, size>& rOpnt)
	{
		for (size_t i = 0; i < size; i++)
			data[i] -= rOpnt.data[i];
		return *static_cast<Vector<T, size>*>(this);
	}

	template<typename T, int size>
	Vector<T, size> IVector<T, size>::operator*(const Vector<T, size>& rOpnt) const
	{
		Vector<T, size> res;
		for (size_t i = 0; i < size; i++)
			res.data[i] = data[i] * rOpnt.data[i];
		return res;
	}

	template<typename T, int size>
	Vector<T, size>& IVector<T, size>::operator*=(const Vector<T, size>& rOpnt)
	{
		for (size_t i = 0; i < size; i++)
			data[i] *= rOpnt.data[i];
		return *static_cast<Vector<T, size>*>(this);
	}

	template<typename T, int size>
	Vector<T, size> IVector<T, size>::operator/(const Vector<T, size>& rOpnt) const
	{
		Vector<T, size> res;
		for (size_t i = 0; i < size; i++)
		{
			if (rOpnt.data[i] == 0)
			{
				std::cerr << "ERROR | XGL::IVector::operator/(const Vector<T, size>&) : Division by zero.\n";
				throw DIVISION_BY_ZERO;
			}
			res.data[i] = data[i] / rOpnt.data[i];
		}
		return res;
	}

	template<typename T, int size>
	Vector<T, size>& IVector<T, size>::operator/=(const Vector<T, size>& rOpnt)
	{
		for (size_t i = 0; i < size; i++)
		{
			if (rOpnt.data[i] == 0)
			{
				std::cerr << "ERROR | XGL::IVector::operator/=(const Vector<T, size>&) : Division by zero.\n";
				throw DIVISION_BY_ZERO;
			}
			data[i] /= rOpnt.data[i];
		}
		return *static_cast<Vector<T, size>*>(this);
	}

	template<typename T, int size>
	Vector<T, size> IVector<T, size>::operator+(T rOpnt) const
	{
		Vector<T, size> res;
		for (size_t i = 0; i < size; i++)
			res.data[i] = data[i] + rOpnt;
		return res;
	}

	template<typename T, int size>
	Vector<T, size> operator+(T lOpnt, const Vector<T, size>& rOpnt)
	{
		Vector<T, size> res;
		for (size_t i = 0; i < size; i++)
			res.data[i] = lOpnt + rOpnt.data[i];
		return res;
	}

	template<typename T, int size>
	Vector<T, size>& IVector<T, size>::operator+=(T rOpnt)
	{
		for (size_t i = 0; i < size; i++)
			data[i] += rOpnt;
		return *static_cast<Vector<T, size>*>(this);
	}

	template<typename T, int size>
	Vector<T, size> IVector<T, size>::operator-(T rOpnt) const
	{
		Vector<T, size> res;
		for (size_t i = 0; i < size; i++)
			res.data[i] = data[i] - rOpnt;
		return res;
	}

	template<typename T, int size>
	Vector<T, size> operator-(T lOpnt, const Vector<T, size>& rOpnt)
	{
		Vector<T, size> res;
		for (size_t i = 0; i < size; i++)
			res.data[i] = lOpnt - rOpnt.data[i];
		return res;
	}

	template<typename T, int size>
	Vector<T, size>& IVector<T, size>::operator-=(T rOpnt)
	{
		for (size_t i = 0; i < size; i++)
			data[i] -= rOpnt;
		return *static_cast<Vector<T, size>*>(this);
	}

	template<typename T, int size>
	Vector<T, size> IVector<T, size>::operator*(T rOpnt) const
	{
		Vector<T, size> res;
		for (size_t i = 0; i < size; i++)
			res.data[i] = data[i] * rOpnt;
		return res;
	}

	template<typename T, int size>
	Vector<T, size> operator*(T lOpnt, const Vector<T, size>& rOpnt)
	{
		Vector<T, size> res;
		for (size_t i = 0; i < size; i++)
			res.data[i] = lOpnt * rOpnt.data[i];
		return res;
	}

	template<typename T, int size>
	Vector<T, size>& IVector<T, size>::operator*=(T rOpnt)
	{
		for (size_t i = 0; i < size; i++)
			data[i] *= rOpnt;
		return *static_cast<Vector<T, size>*>(this);
	}

	template<typename T, int size>
	Vector<T, size> IVector<T, size>::operator/(T rOpnt) const
	{
		if (rOpnt == 0)
		{
			std::cerr << "ERROR | XGL::IVector::operator/(T) : Division by zero.\n";
			throw DIVISION_BY_ZERO;
		}
		Vector<T, size> res;
		for (size_t i = 0; i < size; i++)
			res.data[i] = data[i] / rOpnt;
		return res;
	}

	template<typename T, int size>
	Vector<T, size> operator/(T lOpnt, const Vector<T, size>& rOpnt)
	{
		Vector<T, size> res;
		for (size_t i = 0; i < size; i++)
		{
			if (rOpnt.data[i] == 0)
			{
				std::cerr << "ERROR | XGL::operator/(T, const Vector<T, size>&) : Division by zero.\n";
				throw IVector<T, size>::DIVISION_BY_ZERO;
			}
			res.data[i] = lOpnt / rOpnt.data[i];
		}
		return res;
	}

	template<typename T, int size>
	Vector<T, size>& IVector<T, size>::operator/=(T rOpnt)
	{
		if (rOpnt == 0)
		{
			std::cerr << "ERROR | XGL::IVector::operator/=(T) : Division by zero.\n";
			throw DIVISION_BY_ZERO;
		}
		for (size_t i = 0; i < size; i++)
			data[i] /= rOpnt;
		return *static_cast<Vector<T, size>*>(this);
	}

	template<typename T, int size>
	bool IVector<T, size>::operator==(const Vector<T, size>& rOpnt) const
	{
		for (size_t i = 0; i < size; i++)
			if (data[i] != rOpnt.data[i])
				return false;
		return true;
	}

	template<typename T, int size>
	bool IVector<T, size>::operator!=(const Vector<T, size>& rOpnt) const
	{
		return !((*this) == rOpnt);
	}

	template<typename T, int size>
	T IVector<T, size>::dot(const Vector<T, size>& lOpnt, const Vector<T, size>& rOpnt)
	{
		T res = 0;
		for (size_t i = 0; i < size; i++)
			res += lOpnt.data[i] * rOpnt.data[i];
		return res;
	}

	template<typename T, int size>
	T IVector<T, size>::dot(const Vector<T, size>& rOpnt) const
	{
		T res = 0;
		for (size_t i = 0; i < size; i++)
			res += data[i] * rOpnt.data[i];
		return res;
	}

	template<typename T, int size>
	T IVector<T, size>::norm2(const Vector<T, size>& Opnt)
	{
		return dot(Opnt, Opnt);
	}

	template<typename T, int size>
	T IVector<T, size>::norm2() const
	{
		return dot(*static_cast<const Vector<T, size>*>(this));
	}

	template<typename T, int size>
	T IVector<T, size>::norm(const Vector<T, size>& Opnt)
	{
		return sqrt(norm2(Opnt));
	}

	template<typename T, int size>
	T IVector<T, size>::norm() const
	{
		return sqrt(norm2());
	}

	template<typename T, int size>
	Vector<T, size> IVector<T, size>::normalize(const Vector<T, size>& Opnt)
	{
		T len = norm(Opnt);
		if (len == 0)
		{
			std::cerr << "ERROR | XGL::IVector::normalize(const Vector<T, size>&) : Zero vector cannot be normalized.\n";
			throw ZERO_VECTOR;
		}
		return static_cast<IVector<T, size>>(Opnt) / len;
	}

	template<typename T, int size>
	Vector<T, size> IVector<T, size>::normalize() const
	{
		T len = norm();
		if (len == 0)
		{
			std::cerr << "ERROR | XGL::IVector::normalize() : Zero vector cannot be normalized.\n";
			throw ZERO_VECTOR;
		}
		return *this / len;
	}

	template<typename T, int size>
	Vector<T, size>& IVector<T, size>::fill(T elem)
	{
		for (size_t i = 0; i < size; i++)
			data[i] = elem;
		return *static_cast<Vector<T, size>*>(this);
	}

	template<typename T, int size>
	std::ostream& operator<<(std::ostream& output, const Vector<T, size>& rOpnt)
	{
		output << "( " << rOpnt.data[0];
		for (size_t i = 1; i < size; i++)
			output << ", " << rOpnt.data[i];
		output << " )";
		return output;
	}

	// ------- vec3 -------

	template<typename T>
	Vector<T, 3> Vector<T, 3>::cross(const Vector<T, 3>& lOpnt, const Vector<T, 3>& rOpnt)
	{
		Vector<T, 3> res;
		res.data[0] = lOpnt.data[1] * rOpnt.data[2] - lOpnt.data[2] * rOpnt.data[1];
		res.data[1] = lOpnt.data[2] * rOpnt.data[0] - lOpnt.data[0] * rOpnt.data[2];
		res.data[2] = lOpnt.data[0] * rOpnt.data[1] - lOpnt.data[1] * rOpnt.data[0];
		return res;
	}

	template<typename T>
	Vector<T, 3> Vector<T, 3>::cross(const Vector<T, 3>& rOpnt) const
	{
		Vector<T, 3> res;
		res.data[0] = this->data[1] * rOpnt.data[2] - this->data[2] * rOpnt.data[1];
		res.data[1] = this->data[2] * rOpnt.data[0] - this->data[0] * rOpnt.data[2];
		res.data[2] = this->data[0] * rOpnt.data[1] - this->data[1] * rOpnt.data[0];
		return res;
	}
}

#endif // !XGL_VECTOR_INL
