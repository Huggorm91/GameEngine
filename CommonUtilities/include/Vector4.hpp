#pragma once
#include <assert.h>
#include <cmath>
#include "Vector3.hpp"

//
// Define USE_UNSAFE_CONVERSION to allow conversion to anything with Constructor(T, T, T, T)
// #define USE_UNSAFE_CONVERSION
// 
// Also define USE_INIT_LIST_CONVERSION to allow conversion to anything with 4 public <T> variables
// #define USE_INIT_LIST_CONVERSION
//

namespace CommonUtilities
{
	template <typename T>
	class Vector4
	{
	public:
		T x;
		T y;
		T z;
		T w;

		static const Vector4<T> Null;

		Vector4();
		Vector4(const T& aX, const T& aY, const T& aZ, const T& aW);
		Vector4(const Vector2<T>& aVector2, const T& aZ = T(), const T& aW = T());
		Vector4(const Vector2<T>& aVector2XY, const Vector2<T>& aVector2ZW);
		Vector4(const Vector3<T>& aVector3, const T& aW = T());
		Vector4(const Vector4<T>& aVector) = default;
		Vector4(Vector4<T>&& aVector) = default;
		Vector4<T>& operator=(const Vector4<T>& aVector) = default;
		Vector4<T>& operator=(Vector4<T>&& aVector) = default;
		~Vector4() = default;

		template <class U>operator Vector3<U>() const;
		template <class U>operator Vector4<U>() const;
#ifdef USE_UNSAFE_CONVERSION
		template <class U> operator U() const;
#endif

		inline T LengthSqr() const;
		inline T Length() const;
		inline Vector4<T> GetNormalized() const;
		inline void Normalize();
		inline T Dot(const Vector4<T>& aVector) const;

		inline void Zero();
		inline Vector4<T> Abs();
		inline Vector4<T> ClampMagnitude(T aMaxLength);
		inline Vector4<T> Clamp(T aMin, T aMax);
		inline Vector4<T> Clamp(const Vector4<T>& aMin, const Vector4<T>& aMax);

		inline static Vector4<T> Lerp(const Vector4<T>& aFrom, const Vector4<T>& aTo, float aPercentage);

		Vector4<T> operator-() const;

		inline Vector4& operator+=(const Vector4<T>& aVector);
		inline Vector4 operator+(const Vector4<T>& aVector) const;
		inline Vector4& operator+=(const T& aScalar);
		inline Vector4 operator+(const T& aScalar) const;

		inline Vector4& operator-=(const Vector4<T>& aVector);
		inline Vector4 operator-(const Vector4<T>& aVector) const;
		inline Vector4& operator-=(const T& aScalar);
		inline Vector4 operator-(const T& aScalar) const;

		inline Vector4& operator*= (const Vector4<T>& aVector);
		inline Vector4 operator*(const Vector4<T>& aVector) const;
		inline Vector4<T>& operator*=(const T& aScalar);
		inline Vector4<T> operator*(const T& aScalar) const;

		inline Vector4& operator/=(const Vector4<T>& aVector);
		inline Vector4 operator/(const Vector4<T>& aVector) const;
		inline Vector4& operator/=(const T& aScalar);
		inline Vector4 operator/(const T& aScalar) const;

		inline bool operator==(const Vector4<T>& aVector) const;
		inline bool operator!=(const Vector4<T>& aVector) const;
	};

	template<typename T> const Vector4<T> Vector4<T>::Null{};

	typedef Vector4<float> Vector4f;
	typedef Vector4<unsigned int> Vector4ui;
	typedef Vector4<int>  Vector4i;

	template <typename T> Vector4<T> operator+(const T& aScalar, const Vector4<T>& aVector) {
		return aVector + aScalar;
	}
	template <typename T> Vector4<T> operator-(const T& aScalar, const Vector4<T>& aVector) {
		return aVector - aScalar;
	}
	template <typename T> Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector) {
		return aVector * aScalar;
	}
	template <typename T> Vector4<T> operator/(const T& aScalar, const Vector4<T>& aVector) {
		return aVector / aScalar;
	}

	template<typename T>
	inline Vector4<T>::Vector4() : x(), y(), z(), w()
	{
	}

	template <typename T>
	inline Vector4<T>::Vector4(const T& aX, const T& aY, const T& aZ, const T& aW) : x(aX), y(aY), z(aZ), w(aW)
	{
	}

	template<typename T>
	inline Vector4<T>::Vector4(const Vector2<T>& aVector2, const T& aZ, const T& aW) : x(aVector2.x), y(aVector2.y), z(aZ), w(aW)
	{
	}

	template<typename T>
	inline Vector4<T>::Vector4(const Vector2<T>& aVector2XY, const Vector2<T>& aVector2ZW) : x(aVector2XY.x), y(aVector2XY.y), z(aVector2ZW.x), w(aVector2ZW.y)
	{
	}

	template<typename T>
	inline Vector4<T>::Vector4(const Vector3<T>& aVector3, const T& aW) : x(aVector3.x), y(aVector3.y), z(aVector3.z), w(aW)
	{
	}

	template<typename T>
	template<class U>
	inline Vector4<T>::operator Vector3<U>() const
	{
		return { static_cast<U>(x), static_cast<U>(y), static_cast<U>(z) };
	}

	template<typename T>
	template<class U>
	inline Vector4<T>::operator Vector4<U>() const
	{
		return { static_cast<U>(x), static_cast<U>(y), static_cast<U>(z), static_cast<U>(w) };
	}

	template <typename T>
	inline T Vector4<T>::LengthSqr() const
	{
		return (x * x) + (y * y) + (z * z) + (w * w);
	}

	template <typename T>
	inline T Vector4<T>::Length() const
	{
		return static_cast<T>(sqrt(LengthSqr()));
	}

	template <typename T>
	inline Vector4<T> Vector4<T>::GetNormalized() const
	{
		Vector4 normalized(*this);
		normalized.Normalize();
		return normalized;
	}

	template <typename T>
	inline void Vector4<T>::Normalize()
	{
		T length = Length();
		assert(length != 0);
		if (length == 0)
		{
			return;
		}
		*this /= length;
	}

	template <typename T>
	inline T Vector4<T>::Dot(const Vector4<T>& aVector) const
	{
		return T((x * aVector.x) + (y * aVector.y) + (z * aVector.z) + (w * aVector.w));
	}

	template<typename T>
	inline void Vector4<T>::Zero()
	{
		x = T();
		y = T();
		z = T();
		w = T();
	}

	template<typename T>
	inline Vector4<T> Vector4<T>::Abs()
	{
		return Vector4<T>(CommonUtilities::Abs(x), CommonUtilities::Abs(y), CommonUtilities::Abs(z), CommonUtilities::Abs(w));
	}

	template<typename T>
	inline Vector4<T> Vector4<T>::ClampMagnitude(T aMaxLength)
	{
		T length = Length();
		float multiplier = 1.f;
		if (aMaxLength < length)
		{
			multiplier = aMaxLength / length;
		}
		return Vector4<T>(x * multiplier, y * multiplier, z * multiplier, w * multiplier);
	}

	template<typename T>
	inline Vector4<T> Vector4<T>::Clamp(T aMin, T aMax)
	{
		return Vector4<T>(CommonUtilities::Clamp(x, aMin, aMax),
						  CommonUtilities::Clamp(y, aMin, aMax),
						  CommonUtilities::Clamp(z, aMin, aMax),
						  CommonUtilities::Clamp(w, aMin, aMax));
	}

	template<typename T>
	inline Vector4<T> Vector4<T>::Clamp(const Vector4<T>& aMin, const Vector4<T>& aMax)
	{
		return Vector4<T>(CommonUtilities::Clamp(x, aMin.x, aMax.x),
						  CommonUtilities::Clamp(y, aMin.y, aMax.y),
						  CommonUtilities::Clamp(z, aMin.z, aMax.z),
						  CommonUtilities::Clamp(w, aMin.w, aMax.w));
	}

	template<typename T>
	inline Vector4<T> Vector4<T>::Lerp(const Vector4<T>& aFrom, const Vector4<T>& aTo, float aPercentage)
	{
		return Vector4<T>(CommonUtilities::Lerp(aFrom.x, aTo.x, aPercentage),
						  CommonUtilities::Lerp(aFrom.y, aTo.y, aPercentage),
						  CommonUtilities::Lerp(aFrom.z, aTo.z, aPercentage),
						  CommonUtilities::Lerp(aFrom.w, aTo.w, aPercentage));
	}

	template <typename T>
	inline Vector4<T> Vector4<T>::operator-() const
	{
		return { -x, -y, -z, -w };
	}

	template <typename T>
	inline Vector4<T>& Vector4<T>::operator+=(const Vector4<T>& aVector)
	{
		x += aVector.x;
		y += aVector.y;
		z += aVector.z;
		w += aVector.w;
		return *this;
	}

	template <typename T>
	inline Vector4<T> Vector4<T>::operator+(const Vector4<T>& aVector) const
	{
		Vector4 result = *this;
		return result += aVector;
	}

	template <typename T>
	inline Vector4<T>& Vector4<T>::operator+=(const T& aScalar)
	{
		x += aScalar;
		y += aScalar;
		z += aScalar;
		w += aScalar;
		return *this;
	}

	template <typename T>
	inline Vector4<T> Vector4<T>::operator+(const T& aScalar) const
	{
		Vector4 result = *this;
		return result += aScalar;
	}

	template <typename T>
	inline Vector4<T>& Vector4<T>::operator-=(const Vector4<T>& aVector)
	{
		x -= aVector.x;
		y -= aVector.y;
		z -= aVector.z;
		w -= aVector.w;
		return *this;
	}

	template <typename T>
	inline Vector4<T> Vector4<T>::operator-(const Vector4<T>& aVector) const
	{
		Vector4 result = *this;
		return result -= aVector;
	}

	template <typename T>
	inline Vector4<T>& Vector4<T>::operator-=(const T& aScalar)
	{
		x -= aScalar;
		y -= aScalar;
		z -= aScalar;
		w -= aScalar;
		return *this;
	}

	template <typename T>
	inline Vector4<T> Vector4<T>::operator-(const T& aScalar) const
	{
		Vector4 result = *this;
		return result -= aScalar;
	}

	template <typename T>
	inline Vector4<T>& Vector4<T>::operator*=(const Vector4<T>& aVector)
	{
		x *= aVector.x;
		y *= aVector.y;
		z *= aVector.z;
		w *= aVector.w;
		return *this;
	}

	template <typename T>
	inline Vector4<T> Vector4<T>::operator*(const Vector4<T>& aVector) const
	{
		Vector4 result = *this;
		return result *= aVector;
	}

	template<typename T>
	inline Vector4<T>& Vector4<T>::operator*=(const T& aScalar)
	{
		x *= aScalar;
		y *= aScalar;
		z *= aScalar;
		w *= aScalar;
		return *this;
	}

	template<typename T>
	inline Vector4<T> Vector4<T>::operator*(const T& aScalar) const
	{
		Vector4 result = *this;
		return result *= aScalar;
	}

	template <typename T>
	inline Vector4<T>& Vector4<T>::operator/=(const Vector4<T>& aVector)
	{
		assert(aVector.x != 0 && aVector.y != 0 && aVector.z != 0 && aVector.w != 0);
		if (aVector.x == 0 || aVector.y == 0 || aVector.z == 0 || aVector.w == 0)
		{
			return *this;
		}

		x /= aVector.x;
		y /= aVector.y;
		z /= aVector.z;
		w /= aVector.w;
		return *this;
	}

	template <typename T>
	inline Vector4<T> Vector4<T>::operator/(const Vector4<T>& aVector) const
	{
		Vector4 result = *this;
		return result /= aVector;
	}

	template <typename T>
	inline Vector4<T>& Vector4<T>::operator/=(const T& aScalar)
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
		w = static_cast<T>(w * invert);
		return *this;
	}

	template <typename T>
	inline Vector4<T> Vector4<T>::operator/(const T& aScalar) const
	{
		Vector4 result = *this;
		return result /= aScalar;
	}

	template <typename T>
	inline bool Vector4<T>::operator==(const Vector4<T>& aVector) const
	{
		if (x == aVector.x && y == aVector.y && z == aVector.z && w == aVector.w)
		{
			return true;
		}
		return false;
	}

	template <typename T>
	inline bool Vector4<T>::operator!=(const Vector4<T>& aVector) const
	{
		if (*this == aVector)
		{
			return false;
		}
		return true;
	}
#ifdef USE_UNSAFE_CONVERSION
	template <typename T>
	template<class U>
	inline Vector4<T>::operator U() const
	{
#ifdef USE_INIT_LIST_CONVERSION
		return { x, y, z, w }; // Convertible to structs
#else
		return U(x, y, z, w); // Convertible to classes with Constructor(T, T, T, T)
#endif	
	}
#endif
}