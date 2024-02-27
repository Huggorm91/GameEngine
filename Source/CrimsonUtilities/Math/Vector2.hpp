#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <istream>
#include <ostream>
#include "Math.hpp"

// In order to convert to and from Json you need to include "Json/JsonVector.hpp"
namespace Json
{
	class Value;
}

// 
// Define USE_INIT_LIST_CONVERSION to allow conversion to anything with 2 public <T> variables
// #define USE_INIT_LIST_CONVERSION
//


namespace Crimson
{
	template <typename T>
	class Vector2
	{
	public:
		T x;
		T y;

		static const Vector2<T> Null;
		static const Vector2<T> Up;
		static const Vector2<T> Right;

		Vector2();
		explicit Vector2(const T& aScalar);
		Vector2(const T& aX, const T& aY);
		Vector2(const POINT& aPoint);
		Vector2(const POINTS& aPoints);
		Vector2(const SIZE& aSize);
		Vector2(const std::array<T, 2>& anArray);
		Vector2(const Vector2<T>& aVector) = default;
		Vector2(Vector2<T>&& aVector) = default;
		Vector2<T>& operator=(const Vector2<T>& aVector) = default;
		Vector2<T>& operator=(Vector2<T>&& aVector) = default;
		~Vector2() = default;

		operator POINT() const;
		operator POINTS() const;
		template <class U> operator Vector2<U>() const;
		template <class U> explicit operator U() const;

		inline T LengthSqr() const;
		inline T Length() const;
		inline Vector2<T> GetNormalized() const;
		inline Vector2<T> GetNormalizedNoAssert() const;
		inline void Normalize();
		inline void NormalizeNoAssert();
		inline T Dot(const Vector2<T>& aVector) const;

		inline void Transform(const Vector2<T>& aDistance, const double& aRotation);
		inline void Transform(const Vector2<T>& aDistance, const float& aRotation);
		inline Vector2<T> GetTransformed(const Vector2<T>& aDistance, const double& aRotation);
		inline Vector2<T> GetTransformed(const Vector2<T>& aDistance, const float& aRotation);

		inline void Zero();
		inline Vector2<T> Abs();
		inline Vector2<T> ClampMagnitude(T aMaxLength);
		inline Vector2<T> Clamp(T aMin, T aMax);
		inline Vector2<T> Clamp(const Vector2<T>& aMin, const Vector2<T>& aMax);

		void Serialize(std::ostream& aStream) const;
		void Deserialize(std::istream& aStream);

		inline Vector2<T> operator-() const;

		inline Vector2<T>& operator+=(const Vector2<T>& aVector);
		inline Vector2<T> operator+(const Vector2<T>& aVector) const;
		inline Vector2<T>& operator+=(const T& aScalar);
		inline Vector2<T> operator+(const T& aScalar) const;

		inline Vector2<T>& operator-=(const Vector2<T>& aVector);
		inline Vector2<T> operator-(const Vector2<T>& aVector) const;
		inline Vector2<T>& operator-=(const T& aScalar);
		inline Vector2<T> operator-(const T& aScalar) const;

		inline Vector2<T>& operator*= (const Vector2<T>& aVector);
		inline Vector2<T> operator*(const Vector2<T>& aVector) const;
		inline Vector2<T>& operator*=(const T& aScalar);
		inline Vector2<T> operator*(const T& aScalar) const;

		inline Vector2<T>& operator/=(const Vector2<T>& aVector);
		inline Vector2<T> operator/(const Vector2<T>& aVector) const;
		inline Vector2<T>& operator/=(const T& aScalar);
		inline Vector2<T> operator/(const T& aScalar) const;

		inline bool operator==(const Vector2<T>& aVector) const;
		inline bool operator!=(const Vector2<T>& aVector) const;

		// In order to convert to and from Json you need to include "Json/JsonVector.hpp"
		explicit Vector2(Json::Value aJson);
		explicit operator Json::Value() const;
		Json::Value ToJsonColor() const;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename T> const Vector2<T> Vector2<T>::Null{};
	template<typename T> const Vector2<T> Vector2<T>::Up{ T(), T(1) };
	template<typename T> const Vector2<T> Vector2<T>::Right{ T(1), T() };

	typedef Vector2<float> Vector2f;
	typedef Vector2<unsigned int> Vector2ui;
	typedef Vector2<int>  Vector2i;

	template<typename T>
	inline Vector2<T> Lerp(const Vector2<T>& aFrom, const Vector2<T>& aTo, float aPercentage)
	{
		return Vector2<T>(Crimson::Lerp(aFrom.x, aTo.x, aPercentage),
						  Crimson::Lerp(aFrom.y, aTo.y, aPercentage));
	}

	template <typename T> Vector2<T> operator+(const T& aScalar, const Vector2<T>& aVector)
	{
		return aVector + aScalar;
	}
	template <typename T> Vector2<T> operator-(const T& aScalar, const Vector2<T>& aVector)
	{
		return aVector - aScalar;
	}
	template <typename T> Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector)
	{
		return aVector * aScalar;
	}
	template <typename T> Vector2<T> operator/(const T& aScalar, const Vector2<T>& aVector)
	{
		return aVector / aScalar;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename T>
	inline Vector2<T>::Vector2() : x(), y()
	{}

	template<typename T>
	inline Vector2<T>::Vector2(const T& aScalar) : x(aScalar), y(aScalar)
	{}

	template <typename T>
	inline Vector2<T>::Vector2(const T& aX, const T& aY) : x(aX), y(aY)
	{}

	template <typename T>
	inline Vector2<T>::Vector2(const POINT& aPoint) : x(static_cast<T>(aPoint.x)), y(static_cast<T>(aPoint.y))
	{}

	template <typename T>
	inline Vector2<T>::Vector2(const POINTS& aPoints) : x(static_cast<T>(aPoints.x)), y(static_cast<T>(aPoints.y))
	{}

	template<typename T>
	inline Vector2<T>::Vector2(const SIZE& aSize) : x(static_cast<T>(aSize.cx)), y(static_cast<T>(aSize.cy))
	{}

	template<typename T>
	inline Vector2<T>::Vector2(const std::array<T, 2>& anArray) : x(anArray[0]), y(anArray[1])
	{}

	template <typename T>
	inline Vector2<T>::operator POINT() const
	{
		POINT point;
		point.x = static_cast<long>(x);
		point.y = static_cast<long>(y);
		return point;
	}

	template <typename T>
	inline Vector2<T>::operator POINTS() const
	{
		POINTS points;
		points.x = static_cast<short>(x);
		points.y = static_cast<short>(y);
		return points;
	}

	template<typename T>
	template<class U>
	inline Vector2<T>::operator Vector2<U>() const
	{
		return { static_cast<U>(x), static_cast<U>(y) };
	}

	template <typename T>
	inline T Vector2<T>::LengthSqr() const
	{
		return (x * x) + (y * y);
	}

	template <typename T>
	inline T Vector2<T>::Length() const
	{
		return static_cast<T>(sqrt(LengthSqr()));
	}

	template <typename T>
	inline Vector2<T> Vector2<T>::GetNormalized() const
	{
		Vector2 normalized(*this);
		normalized.Normalize();
		return normalized;
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::GetNormalizedNoAssert() const
	{
		Vector2 normalized(*this);
		normalized.NormalizeNoAssert();
		return normalized;
	}

	template <typename T>
	inline void Vector2<T>::Normalize()
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
	inline void Vector2<T>::NormalizeNoAssert()
	{
		T length = Length();
		if (length == 0)
		{
			return;
		}
		*this /= length;
	}

	template <typename T>
	inline T Vector2<T>::Dot(const Vector2<T>& aVector) const
	{
		return T((x * aVector.x) + (y * aVector.y));
	}

	template<typename T>
	inline void Vector2<T>::Transform(const Vector2<T>& aDistance, const double& aRotation)
	{
		Crimson::Vector2<double> newXBasis(cos(aRotation), -sin(aRotation));
		Crimson::Vector2<double> newYBasis(sin(aRotation), cos(aRotation));

		x += aDistance.Dot(newXBasis);
		y += aDistance.Dot(newYBasis);
	}

	template<typename T>
	inline void Vector2<T>::Transform(const Vector2<T>& aDistance, const float& aRotation)
	{
		Crimson::Vector2<float> newXBasis(cos(aRotation), -sin(aRotation));
		Crimson::Vector2<float> newYBasis(sin(aRotation), cos(aRotation));

		x += aDistance.Dot(newXBasis);
		y += aDistance.Dot(newYBasis);
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::GetTransformed(const Vector2<T>& aDistance, const double& aRotation)
	{
		Vector2<T> transformed(*this);
		transformed.Transform(aDistance, aRotation);
		return transformed;
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::GetTransformed(const Vector2<T>& aDistance, const float& aRotation)
	{
		Vector2<T> transformed(*this);
		transformed.Transform(aDistance, aRotation);
		return transformed;
	}

	template<typename T>
	inline void Vector2<T>::Zero()
	{
		x = T();
		y = T();
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::Abs()
	{
		return Vector2<T>(Crimson::Abs(x), Crimson::Abs(y));
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::ClampMagnitude(T aMaxLength)
	{
		T length = Length();
		float multiplier = 1.f;
		if (aMaxLength < length)
		{
			multiplier = aMaxLength / length;
		}
		return Vector2<T>(x * multiplier, y * multiplier);
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::Clamp(T aMin, T aMax)
	{
		return Vector2<T>(Crimson::Clamp(x, aMin, aMax),
						  Crimson::Clamp(y, aMin, aMax));
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::Clamp(const Vector2<T>& aMin, const Vector2<T>& aMax)
	{
		return Vector2<T>(Crimson::Clamp(x, aMin.x, aMax.x),
						  Crimson::Clamp(y, aMin.y, aMax.y));
	}

	template<typename T>
	inline void Vector2<T>::Serialize(std::ostream& aStream) const
	{
		aStream.write(reinterpret_cast<const char*>(&x), sizeof(x) * 2);
	}

	template<typename T>
	inline void Vector2<T>::Deserialize(std::istream& aStream)
	{
		aStream.read(reinterpret_cast<char*>(&x), sizeof(x) * 2);
	}

	template <typename T>
	inline Vector2<T> Vector2<T>::operator-() const
	{
		return { -x,-y };
	}

	template <typename T>
	inline Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& aVector)
	{
		x += aVector.x;
		y += aVector.y;
		return *this;
	}

	template <typename T>
	inline Vector2<T> Vector2<T>::operator+(const Vector2<T>& aVector) const
	{
		Vector2 result = *this;
		return result += aVector;
	}

	template <typename T>
	inline Vector2<T>& Vector2<T>::operator+=(const T& aScalar)
	{
		x += aScalar;
		y += aScalar;
		return *this;
	}

	template <typename T>
	inline Vector2<T> Vector2<T>::operator+(const T& aScalar) const
	{
		Vector2 result = *this;
		return result += aScalar;
	}

	template <typename T>
	inline Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& aVector)
	{
		x -= aVector.x;
		y -= aVector.y;
		return *this;
	}

	template <typename T>
	inline Vector2<T> Vector2<T>::operator-(const Vector2<T>& aVector) const
	{
		Vector2 result = *this;
		return result -= aVector;
	}

	template <typename T>
	inline Vector2<T>& Vector2<T>::operator-=(const T& aScalar)
	{
		x -= aScalar;
		y -= aScalar;
		return *this;
	}

	template <typename T>
	inline Vector2<T> Vector2<T>::operator-(const T& aScalar) const
	{
		Vector2 result = *this;
		return result -= aScalar;
	}

	template <typename T>
	inline Vector2<T>& Vector2<T>::operator*=(const Vector2<T>& aVector)
	{
		x *= aVector.x;
		y *= aVector.y;
		return *this;
	}

	template <typename T>
	inline Vector2<T> Vector2<T>::operator*(const Vector2<T>& aVector) const
	{
		Vector2 result = *this;
		return result *= aVector;
	}

	template<typename T>
	inline Vector2<T>& Vector2<T>::operator*=(const T& aScalar)
	{
		x *= aScalar;
		y *= aScalar;
		return *this;
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::operator*(const T& aScalar) const
	{
		Vector2 result = *this;
		return result *= aScalar;
	}

	template <typename T>
	inline Vector2<T>& Vector2<T>::operator/=(const Vector2<T>& aVector)
	{
		assert(aVector.x != 0 && aVector.y != 0);
		if (aVector.x == 0 || aVector.y == 0)
		{
			return *this;
		}

		x /= aVector.x;
		y /= aVector.y;
		return *this;
	}

	template <typename T>
	inline Vector2<T> Vector2<T>::operator/(const Vector2<T>& aVector) const
	{
		Vector2 result = *this;
		return result /= aVector;
	}

	template <typename T>
	inline Vector2<T>& Vector2<T>::operator/=(const T& aScalar)
	{
		assert(aScalar != 0);
		if (aScalar == 0)
		{
			return *this;
		}

		double invert = 1. / static_cast<double>(aScalar);
		x = static_cast<T>(x * invert);
		y = static_cast<T>(y * invert);
		return *this;
	}

	template <typename T>
	inline Vector2<T> Vector2<T>::operator/(const T& aScalar) const
	{
		Vector2 result = *this;
		return result /= aScalar;
	}

	template <typename T>
	inline bool Vector2<T>::operator==(const Vector2<T>& aVector) const
	{
		if (x == aVector.x && y == aVector.y)
		{
			return true;
		}
		return false;
	}

	template <typename T>
	inline bool Vector2<T>::operator!=(const Vector2<T>& aVector) const
	{
		if (*this == aVector)
		{
			return false;
		}
		return true;
	}

	template <typename T>
	template<class U>
	inline Vector2<T>::operator U() const
	{
#ifdef USE_INIT_LIST_CONVERSION
		return { x, y }; // Convertible to structs
#else
		return U(x, y); // Convertible to classes with Constructor(T, T)
#endif
	}
}