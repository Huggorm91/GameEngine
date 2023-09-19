#pragma once
#include <assert.h>
#include <cmath>
#include "Vector2.hpp"

// 
// Also define USE_INIT_LIST_CONVERSION to allow conversion to anything with 3 public <T> variables
// #define USE_INIT_LIST_CONVERSION
//

namespace CommonUtilities
{
	template <typename T>
	class Vector3
	{
	public:
		T x;
		T y;
		T z;

		static const Vector3<T> Null;
		static const Vector3<T> Up;
		static const Vector3<T> Right;
		static const Vector3<T> Forward;

		Vector3();
		explicit Vector3(const T& aScalar);
		Vector3(const T& aX, const T& aY, const T& aZ);
		Vector3(const Vector2<T>& aVector2, const T& aZ = T());
		Vector3(const Vector3<T>& aVector) = default;
		Vector3(Vector3<T>&& aVector) = default;
		Vector3<T>& operator=(const Vector3<T>& aVector) = default;
		Vector3<T>& operator=(Vector3<T>&& aVector) = default;
		~Vector3() = default;

		T& operator[](int value);
		const T& operator[](int value) const;

		template <class U>operator Vector2<U>() const;
		template <class U>operator Vector3<U>() const;
		template <class U> explicit operator U() const;

		inline T LengthSqr() const;
		inline T Length() const;
		inline Vector3<T> GetNormalized() const;
		inline Vector3<T> GetNormalizedNoAssert() const;
		inline void Normalize();
		inline void NormalizeNoAssert();
		inline T Dot(const Vector3<T>& aVector) const;
		inline Vector3<T> Cross(const Vector3<T>& aVector) const;

		inline void Zero();
		inline Vector3<T> Abs();
		inline Vector3<T> ClampMagnitude(T aMaxLength);
		inline Vector3<T> Clamp(T aMin, T aMax);
		inline Vector3<T> Clamp(const Vector3<T>& aMin, const Vector3<T>& aMax);

		inline static Vector3<T> Lerp(const Vector3<T>& aFrom, const Vector3<T>& aTo, float aPercentage);

		Vector3<T> operator-() const;

		inline Vector3<T>& operator+=(const Vector3<T>& aVector);
		inline Vector3<T> operator+(const Vector3<T>& aVector) const;
		inline Vector3<T>& operator+=(const T& aScalar);
		inline Vector3<T> operator+(const T& aScalar) const;

		inline Vector3<T>& operator-=(const Vector3<T>& aVector);
		inline Vector3<T> operator-(const Vector3<T>& aVector) const;
		inline Vector3<T>& operator-=(const T& aScalar);
		inline Vector3<T> operator-(const T& aScalar) const;

		inline Vector3<T>& operator*= (const Vector3<T>& aVector);
		inline Vector3<T> operator*(const Vector3<T>& aVector) const;
		inline Vector3<T>& operator*=(const T& aScalar);
		inline Vector3<T> operator*(const T& aScalar) const;

		inline Vector3<T>& operator/=(const Vector3<T>& aVector);
		inline Vector3<T> operator/(const Vector3<T>& aVector) const;
		inline Vector3<T>& operator/=(const T& aScalar);
		inline Vector3<T> operator/(const T& aScalar) const;

		inline bool operator==(const Vector3<T>& aVector) const;
		inline bool operator!=(const Vector3<T>& aVector) const;

		// In order to convert to and from Json you need to include "JsonVector.hpp"
		explicit Vector3(Json::Value aJson);
		explicit operator Json::Value() const;
		Json::Value ToJsonColor() const;
	};

	template<typename T> const Vector3<T> Vector3<T>::Null{};
	template<typename T> const Vector3<T> Vector3<T>::Up{T(), T(1), T()};
	template<typename T> const Vector3<T> Vector3<T>::Right{T(1), T(), T()};
	template<typename T> const Vector3<T> Vector3<T>::Forward{T(), T(), T(1)};

	typedef Vector3<float> Vector3f;
	typedef Vector3<unsigned int> Vector3ui;
	typedef Vector3<int>  Vector3i;

	template <typename T>
	Vector3<float> DegreeToRadian(const Vector3<T>& aDegree)
	{
		return static_cast<Vector3<float>>(aDegree) * static_cast<float>(globalDegreeToRadianMultiplier);
	}
	template <typename T>
	Vector3<double> DegreeToRadianPrecise(const Vector3<T>& aDegree)
	{
		return static_cast<Vector3<double>>(aDegree) * globalDegreeToRadianMultiplier;
	}
	template <typename T>
	Vector3<float> RadianToDegree(const Vector3<T>& aRadian)
	{
		return static_cast<Vector3<float>>(aRadian) * static_cast<float>(globalRadianToDegreeMultiplier);
	}
	template <typename T>
	Vector3<double> RadianToDegreePrecise(const Vector3<T>& aRadian)
	{
		return static_cast<Vector3<double>>(aRadian) * globalRadianToDegreeMultiplier;
	}

	template <typename T> Vector3<T> operator+(const T& aScalar, const Vector3<T>& aVector) {
		return aVector + aScalar;
	}
	template <typename T> Vector3<T> operator-(const T& aScalar, const Vector3<T>& aVector) {
		return aVector - aScalar;
	}
	template <typename T> Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector) {
		return aVector * aScalar;
	}
	template <typename T> Vector3<T> operator/(const T& aScalar, const Vector3<T>& aVector) {
		return aVector / aScalar;
	}

	template<typename T>
	inline Vector3<T>::Vector3() : x(), y(), z()
	{
	}

	template<typename T>
	inline Vector3<T>::Vector3(const T& aScalar) : x(aScalar), y(aScalar), z(aScalar)
	{
	}

	template <typename T>
	inline Vector3<T>::Vector3(const T& aX, const T& aY, const T& aZ) : x(aX), y(aY), z(aZ)
	{
	}

	template<typename T>
	inline Vector3<T>::Vector3(const Vector2<T>& aVector2, const T& aZ) : x(aVector2.x), y(aVector2.y), z(aZ)
	{
	}

	template<typename T>
	template<class U>
	inline Vector3<T>::operator Vector2<U>() const
	{
		return { static_cast<U>(x), static_cast<U>(y) };
	}

	template<typename T>
	template<class U>
	inline Vector3<T>::operator Vector3<U>() const
	{
		return { static_cast<U>(x), static_cast<U>(y), static_cast<U>(z) };
	}

	template <typename T>
	inline T Vector3<T>::LengthSqr() const
	{
		return (x * x) + (y * y) + (z * z);
	}

	template <typename T>
	inline T Vector3<T>::Length() const
	{
		return static_cast<T>(sqrt(LengthSqr()));
	}

	template <typename T>
	inline Vector3<T> Vector3<T>::GetNormalized() const
	{
		Vector3 normalized(*this);
		normalized.Normalize();
		return normalized;
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::GetNormalizedNoAssert() const
	{
		Vector3 normalized(*this);
		normalized.NormalizeNoAssert();
		return normalized;
	}

	template <typename T>
	inline void Vector3<T>::Normalize()
	{
		T length = Length();
		assert(length != 0);
		if (length == 0)
		{
			return;
		}
		*this /= length;
	}

	template<typename T>
	inline void Vector3<T>::NormalizeNoAssert()
	{
		T length = Length();
		if (length == 0)
		{
			return;
		}
		*this /= length;
	}

	template <typename T>
	inline T Vector3<T>::Dot(const Vector3<T>& aVector) const
	{
		return T((x * aVector.x) + (y * aVector.y) + (z * aVector.z));
	}

	template <typename T>
	inline Vector3<T> Vector3<T>::Cross(const Vector3<T>& aVector) const
	{
		Vector3<T> crossProduct;
		crossProduct.x = (y * aVector.z) - (z * aVector.y);
		crossProduct.y = (z * aVector.x) - (x * aVector.z);
		crossProduct.z = (x * aVector.y) - (y * aVector.x);
		return crossProduct;
	}

	template<typename T>
	inline void Vector3<T>::Zero()
	{
		x = T();
		y = T();
		z = T();
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::Abs()
	{
		return Vector3<T>(CommonUtilities::Abs(x), CommonUtilities::Abs(y), CommonUtilities::Abs(z));
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::ClampMagnitude(T aMaxLength)
	{
		T length = Length();
		float multiplier = 1.f;
		if (aMaxLength < length)
		{
			multiplier = aMaxLength / length;
		}
		return Vector3<T>(x * multiplier, y * multiplier, z * multiplier);
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::Clamp(T aMin, T aMax)
	{
		return Vector3<T>(CommonUtilities::Clamp(x, aMin, aMax),
						  CommonUtilities::Clamp(y, aMin, aMax),
						  CommonUtilities::Clamp(z, aMin, aMax));
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::Clamp(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		return Vector3<T>(CommonUtilities::Clamp(x, aMin.x, aMax.x),
						  CommonUtilities::Clamp(y, aMin.y, aMax.y),
						  CommonUtilities::Clamp(z, aMin.z, aMax.z));
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::Lerp(const Vector3<T>& aFrom, const Vector3<T>& aTo, float aPercentage)
	{
		return Vector3<T>(CommonUtilities::Lerp(aFrom.x, aTo.x, aPercentage),
						  CommonUtilities::Lerp(aFrom.y, aTo.y, aPercentage),
						  CommonUtilities::Lerp(aFrom.z, aTo.z, aPercentage));
	}

	template<class T>
	inline T& Vector3<T>::operator[](int value)
	{
		assert(value >= 0 && value <= 2);
		switch (value)
		{
		case 0:
		{
			return x;
		}
		case 1:
		{
			return y;
		}
		case 2:
		{
			return z;
		}
		default:
		{
			return x;
		}
		}
	}

	template<class T>
	inline const T& Vector3<T>::operator[](int value) const
	{
		assert(value >= 0 && value <= 2);
		switch (value)
		{
		case 0:
		{
			return x;
		}
		case 1:
		{
			return y;
		}
		case 2:
		{
			return z;
		}
		default:
		{
			return x;
		}
		}
	}

	template <typename T>
	inline Vector3<T> Vector3<T>::operator-() const
	{
		return { -x, -y, -z };
	}

	template <typename T>
	inline Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& aVector)
	{
		x += aVector.x;
		y += aVector.y;
		z += aVector.z;
		return *this;
	}

	template <typename T>
	inline Vector3<T> Vector3<T>::operator+(const Vector3<T>& aVector) const
	{
		Vector3 result = *this;
		return result += aVector;
	}

	template <typename T>
	inline Vector3<T>& Vector3<T>::operator+=(const T& aScalar)
	{
		x += aScalar;
		y += aScalar;
		z += aScalar;
		return *this;
	}

	template <typename T>
	inline Vector3<T> Vector3<T>::operator+(const T& aScalar) const
	{
		Vector3 result = *this;
		return result += aScalar;
	}

	template <typename T>
	inline Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& aVector)
	{
		x -= aVector.x;
		y -= aVector.y;
		z -= aVector.z;
		return *this;
	}

	template <typename T>
	inline Vector3<T> Vector3<T>::operator-(const Vector3<T>& aVector) const
	{
		Vector3 result = *this;
		return result -= aVector;
	}

	template <typename T>
	inline Vector3<T>& Vector3<T>::operator-=(const T& aScalar)
	{
		x -= aScalar;
		y -= aScalar;
		z -= aScalar;
		return *this;
	}

	template <typename T>
	inline Vector3<T> Vector3<T>::operator-(const T& aScalar) const
	{
		Vector3 result = *this;
		return result -= aScalar;
	}

	template <typename T>
	inline Vector3<T>& Vector3<T>::operator*=(const Vector3<T>& aVector)
	{
		x *= aVector.x;
		y *= aVector.y;
		z *= aVector.z;
		return *this;
	}

	template <typename T>
	inline Vector3<T> Vector3<T>::operator*(const Vector3<T>& aVector) const
	{
		Vector3 result = *this;
		return result *= aVector;
	}

	template<typename T>
	inline Vector3<T>& Vector3<T>::operator*=(const T& aScalar)
	{
		x *= aScalar;
		y *= aScalar;
		z *= aScalar;
		return *this;
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::operator*(const T& aScalar) const
	{
		Vector3 result = *this;
		return result *= aScalar;
	}

	template <typename T>
	inline Vector3<T>& Vector3<T>::operator/=(const Vector3<T>& aVector)
	{
		assert(aVector.x != 0 && aVector.y != 0 && aVector.z != 0);
		if (aVector.x == 0 || aVector.y == 0 || aVector.z == 0)
		{
			return *this;
		}

		x /= aVector.x;
		y /= aVector.y;
		z /= aVector.z;
		return *this;
	}

	template <typename T>
	inline Vector3<T> Vector3<T>::operator/(const Vector3<T>& aVector) const
	{
		Vector3 result = *this;
		return result /= aVector;
	}

	template <typename T>
	inline Vector3<T>& Vector3<T>::operator/=(const T& aScalar)
	{
		assert(aScalar != 0);
		if (aScalar == 0)
		{
			return *this;
		}

		double invert = 1. / static_cast<double>(aScalar);
		x = static_cast<T>(x * invert);
		y = static_cast<T>(y * invert);
		z = static_cast<T>(z * invert);
		return *this;
	}

	template <typename T>
	inline Vector3<T> Vector3<T>::operator/(const T& aScalar) const
	{
		Vector3 result = *this;
		return result /= aScalar;
	}

	template <typename T>
	inline bool Vector3<T>::operator==(const Vector3<T>& aVector) const
	{
		if (x == aVector.x && y == aVector.y && z == aVector.z)
		{
			return true;
		}
		return false;
	}

	template <typename T>
	inline bool Vector3<T>::operator!=(const Vector3<T>& aVector) const
	{
		if (*this == aVector)
		{
			return false;
		}
		return true;
	}

	template <typename T>
	template<class U>
	inline Vector3<T>::operator U() const
	{
#ifdef USE_INIT_LIST_CONVERSION
		return { x, y, z }; // Convertible to structs
#else
		return U(x, y, z); // Convertible to classes with Constructor(T, T, T)
#endif
	}
}