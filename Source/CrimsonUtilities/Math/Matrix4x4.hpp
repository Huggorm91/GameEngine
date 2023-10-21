#pragma once
#include "Vector4.hpp"

namespace Crimson
{
	template <typename T>
	class Matrix4x4
	{
	public:
		T m11;
		T m12;
		T m13;
		T m14;
		T m21;
		T m22;
		T m23;
		T m24;
		T m31;
		T m32;
		T m33;
		T m34;
		T m41;
		T m42;
		T m43;
		T m44;

		static const Matrix4x4<T> Null;

		// Creates the identity matrix.
		Matrix4x4<T>();
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix) = default;
		Matrix4x4<T>(Matrix4x4<T>&& aMatrix) = default;
		Matrix4x4<T>(T aM11, T aM12, T aM13, T aM14, T aM21, T aM22, T aM23, T aM24, T aM31, T aM32, T aM33, T aM34, T aM41, T aM42, T aM43, T aM44);
		Matrix4x4<T>(const Vector4<T>& aFirstRow, const Vector4<T>& aSecondRow = Vector4<T>(T(), T(1), T(), T()), const Vector4<T>& aThirdRow = Vector4<T>(T(), T(), T(1), T()), const Vector4<T>& aFourthRow = Vector4<T>(T(), T(), T(), T(1)));
		~Matrix4x4() = default;

		inline Matrix4x4<T>& operator=(const Matrix4x4<T>& aMatrix) = default;
		inline Matrix4x4<T>& operator=(Matrix4x4<T>&& aMatrix) = default;

		const T& operator()(const unsigned int aRow, const unsigned int aColumn) const;
		T& operator()(const unsigned int aRow, const unsigned int aColumn);

		static Matrix4x4<T> CreateRotationAroundX(const T& anAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(const T& anAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(const T& anAngleInRadians);

		static Matrix4x4<T> CreateScaleMatrix(const Vector3<T>& aScale);
		static Matrix4x4<T> CreateRotationMatrix(const Vector3<T>& aRotation);
		static Matrix4x4<T> CreateTranslationMatrix(const Vector3<T>& aPosition);

		static Matrix4x4<T> CreatePerspectiveMatrix(float aFoVRadian, float aNearPlane, float aFarPlane, const Vector2f& aScreenSize);
		static Matrix4x4<T> CreateOrthographicMatrix(float aLeftPlane, float aRightPlane, float aBottomPlane, float aTopPlane, float aNearPlane, float aFarPlane);
		static Matrix4x4<T> LookAt(const Vector3<T>& aCameraPosition, const Vector3<T>& aTarget, const Vector3<T>& anUpVector);
		static Matrix4x4<T> LookAt(const Vector3<T>& aCameraPosition, const Vector3<T>& aTarget);

		// Assumes aTransform is made up of nothing but rotations and translations.
		inline Matrix4x4<T> GetInverse() const;
		inline Matrix4x4<T> GetTranspose() const;

		inline Vector3<T> GetForward() const;
		inline Vector3<T> GetUp() const;
		inline Vector3<T> GetRight() const;

		inline void SetForward(const Vector3<T>& aVector);
		inline void SetUp(const Vector3<T>& aVector);
		inline void SetRight(const Vector3<T>& aVector);

		Vector3<T> GetEulerAngles() const;

		void Serialize(std::ostream& aStream) const;
		void Deserialize(std::istream& aStream);

		inline Matrix4x4<T>& operator+=(const Matrix4x4<T>& aMatrix);
		inline Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrix) const;
		inline Matrix4x4<T>& operator+=(const T& aScalar);
		inline Matrix4x4<T> operator+(const T& aScalar) const;

		inline Matrix4x4<T>& operator-=(const Matrix4x4<T>& aMatrix);
		inline Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrix) const;
		inline Matrix4x4<T>& operator-=(const T& aScalar);
		inline Matrix4x4<T> operator-(const T& aScalar) const;

		inline Matrix4x4<T>& operator*= (const Matrix4x4<T>& aMatrix);
		inline Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix) const;
		inline Matrix4x4<T>& operator*=(const T& aScalar);
		inline Matrix4x4<T> operator*(const T& aScalar) const;

		inline Vector4<T> operator*(const Vector4<T>& aRowVector) const;

		inline Matrix4x4<T>& operator/=(const T& aScalar);
		inline Matrix4x4<T> operator/(const T& aScalar) const;

		inline bool operator==(const Matrix4x4<T>& aMatrix) const;
		inline bool operator!=(const Matrix4x4<T>& aMatrix) const;
	};

	template<typename T> const Matrix4x4<T> Matrix4x4<T>::Null{};

	typedef Matrix4x4<float> Mat4;
	typedef Matrix4x4<float> Matrix4x4f;
	typedef Matrix4x4<int>  Matrix4x4i;

	template<typename T>
	Vector4<T>& operator*=(Vector4<T>& aRowVector, const Matrix4x4<T>& aMatrix) {
		return aRowVector = aMatrix * aRowVector;
	}
	template<typename T>
	Vector4<T> operator*(const Vector4<T>& aRowVector, const Matrix4x4<T>& aMatrix) {
		return aMatrix * aRowVector;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename T>
	inline Matrix4x4<T>::Matrix4x4() :
		m11(T(1)), m12(T()), m13(T()), m14(T()),
		m21(T()), m22(T(1)), m23(T()), m24(T()),
		m31(T()), m32(T()), m33(T(1)), m34(T()),
		m41(T()), m42(T()), m43(T()), m44(T(1))
	{
	}

	template<typename T>
	Matrix4x4<T>::Matrix4x4(T a11, T a12, T a13, T a14, T a21, T a22, T a23, T a24, T a31, T a32, T a33, T a34, T a41, T a42, T a43, T a44) :
		m11(a11), m12(a12), m13(a13), m14(a14),
		m21(a21), m22(a22), m23(a23), m24(a24),
		m31(a31), m32(a32), m33(a33), m34(a34),
		m41(a41), m42(a42), m43(a43), m44(a44)
	{
	}

	template<typename T>
	inline Matrix4x4<T>::Matrix4x4(const Vector4<T>& aFirstRow, const Vector4<T>& aSecondRow, const Vector4<T>& aThirdRow, const Vector4<T>& aFourthRow) :
		m11(aFirstRow.x), m12(aFirstRow.y), m13(aFirstRow.z), m14(aFirstRow.w),
		m21(aSecondRow.x), m22(aSecondRow.y), m23(aSecondRow.z), m24(aSecondRow.w),
		m31(aThirdRow.x), m32(aThirdRow.y), m33(aThirdRow.z), m34(aThirdRow.w),
		m41(aFourthRow.x), m42(aFourthRow.y), m43(aFourthRow.z), m44(aFourthRow.w)
	{
	}

	template<typename T>
	inline const T& Matrix4x4<T>::operator()(const unsigned int aRow, const unsigned int aColumn) const
	{
		return *((&m11) + ((aRow - 1) * 4) + (aColumn - 1));
	}

	template<typename T>
	inline T& Matrix4x4<T>::operator()(const unsigned int aRow, const unsigned int aColumn)
	{
		return *((&m11) + ((aRow - 1) * 4) + (aColumn - 1));
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(const T& anAngleInRadians)
	{
		return Matrix4x4<T>{
			{ T(1), T(), T(), T() },
			{ T(), T(cos(anAngleInRadians)), T(sin(anAngleInRadians)) , T() },
			{ T(), T(-sin(anAngleInRadians)), T(cos(anAngleInRadians)) , T() },
			{ T(), T(), T(), T(1) }	};
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(const T& anAngleInRadians)
	{
		return Matrix4x4<T>{
			{ T(cos(anAngleInRadians)), T(), T(-sin(anAngleInRadians)), T()},
			{ T(), T(1), T(), T() },
			{ T(sin(anAngleInRadians)), T(), T(cos(anAngleInRadians)), T() },
			{ T(), T(), T(), T(1) }	};
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(const T& anAngleInRadians)
	{
		return Matrix4x4<T>{
			{ T(cos(anAngleInRadians)), T(sin(anAngleInRadians)), T(), T()},
			{ T(-sin(anAngleInRadians)), T(cos(anAngleInRadians)), T(), T() },
			{ T(), T(), T(1), T() },
			{ T(), T(), T(), T(1) }	};
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateScaleMatrix(const Vector3<T>& aScale)
	{
		return Matrix4x4<T>{
			{ T(aScale.x), T(), T(), T() },
			{ T(), T(aScale.y), T(), T() },
			{ T(), T(), T(aScale.z), T() },
			{ T(), T(), T(), T(1) }
		};
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationMatrix(const Vector3<T>& aRotation)
	{
		return CreateRotationAroundX(aRotation.x) * CreateRotationAroundY(aRotation.y) * CreateRotationAroundZ(aRotation.z);
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateTranslationMatrix(const Vector3<T>& aPosition)
	{
		return Matrix4x4<T>{
			{ T(1), T(), T(), T() },
			{ T(), T(1), T(), T() },
			{ T(), T(), T(1), T() },
			{ T(aPosition.x), T(aPosition.y), T(aPosition.z), T(1) }
		};
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreatePerspectiveMatrix(float aFoVRadian, float aNearPlane, float aFarPlane, const Vector2f& aScreenSize)
	{
		const float divideInverse = 1.f / (aFarPlane - aNearPlane);
		const float fov = 1.f / tan(aFoVRadian * 0.5f);

		return Matrix4x4<T>{
			{ T(fov), T(), T(), T() },
			{ T(), T(fov * (aScreenSize.x / aScreenSize.y)), T(), T() },
			{ T(), T(), T(aFarPlane * divideInverse), T(1) },
			{ T(), T(), T(-aNearPlane * aFarPlane * divideInverse), T(0) }};
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateOrthographicMatrix(float aLeftPlane, float aRightPlane, float aBottomPlane, float aTopPlane, float aNearPlane, float aFarPlane)
	{
		const float reciprocalWidth = 1.0f / (aRightPlane - aLeftPlane);
		const float reciprocalHeight = 1.0f / (aTopPlane - aBottomPlane);
		const float fRange = 1.0f / (aFarPlane - aNearPlane);

		return Matrix4x4<T>{
			{ T(reciprocalWidth + reciprocalWidth), T(), T(), T() },
			{ T(), T(reciprocalHeight + reciprocalHeight), T(), T() },
			{ T(), T(), T(fRange), T() },
			{ T(-(aLeftPlane + aRightPlane) * reciprocalWidth), T(-(aTopPlane + aBottomPlane) * reciprocalHeight), T(-fRange * aNearPlane), T(1) }};
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::LookAt(const Vector3<T>& aCameraPosition, const Vector3<T>& aTarget, const Vector3<T>& anUpVector)
	{
		Vector3<T> zAxis = (aTarget - aCameraPosition).GetNormalized();
		Vector3<T> xAxis = Vector3<T>::Null;
		if (zAxis == anUpVector)
		{
			xAxis = (Vector3<T>{ anUpVector.x, anUpVector.z, anUpVector.y }.Cross(zAxis)).GetNormalized();
		}
		else
		{
			xAxis = (anUpVector.Cross(zAxis)).GetNormalized();
		}

		Vector3<T> yAxis = zAxis.Cross(xAxis);
		return Matrix4x4<T>{{ xAxis.x, yAxis.x, zAxis.x, T() },
							{ xAxis.y, yAxis.y, zAxis.y, T() },
							{ xAxis.z, yAxis.z, zAxis.z, T() },
							{ -(xAxis.Dot(aCameraPosition)), -(yAxis.Dot(aCameraPosition)), -(zAxis.Dot(aCameraPosition)), T(1) }};
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::LookAt(const Vector3<T>& aCameraPosition, const Vector3<T>& aTarget)
	{
		Vector3<T> zAxis = (aTarget - aCameraPosition).GetNormalized();
		Vector3<T> xAxis = Vector3<T>::Null;
		if (Abs(zAxis.y) < (1.f - 0.00001f))
		{
			xAxis = (Vector3<T>::Up.Cross(zAxis)).GetNormalized();
		}
		else
		{
			xAxis = (Vector3<T>::Forward.Cross(zAxis)).GetNormalized();
		}

		Vector3<T> yAxis = zAxis.Cross(xAxis);
		return Matrix4x4<T>{{ xAxis.x, yAxis.x, zAxis.x, T() },
							{ xAxis.y, yAxis.y, zAxis.y, T() },
							{ xAxis.z, yAxis.z, zAxis.z, T() },
							{ -(xAxis.Dot(aCameraPosition)), -(yAxis.Dot(aCameraPosition)), -(zAxis.Dot(aCameraPosition)), T(1) }};
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::GetInverse() const
	{
		Matrix4x4<T> result{
			m11, m21, m31, m14,
			m12, m22, m32, m24,
			m13, m23, m33, m34,
			T(), T(), T(), T(1) };
		Vector4<T> translate(-m41, -m42, -m43, m44);
		translate = translate * result;
		result.m41 = translate.x;
		result.m42 = translate.y;
		result.m43 = translate.z;
		return result;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::GetTranspose() const
	{
		return {
			m11, m21, m31, m41,
			m12, m22, m32, m42,
			m13, m23, m33, m43,
			m14, m24, m34, m44
		};
	}

	template<typename T>
	inline Vector3<T> Matrix4x4<T>::GetForward() const
	{
		return Vector3<T>(m13, m23, m33).GetNormalized();
	}

	template<typename T>
	inline Vector3<T> Matrix4x4<T>::GetUp() const
	{
		return Vector3<T>(m12, m22, m32).GetNormalized();
	}

	template<typename T>
	inline Vector3<T> Matrix4x4<T>::GetRight() const
	{
		return Vector3<T>(m11, m21, m31).GetNormalized();
	}

	template<typename T>
	inline void Matrix4x4<T>::SetForward(const Vector3<T>& aVector)
	{
		m13 = aVector.x;
		m23 = aVector.y;
		m33 = aVector.z;
	}

	template<typename T>
	inline void Matrix4x4<T>::SetUp(const Vector3<T>& aVector)
	{
		m12 = aVector.x;
		m22 = aVector.y;
		m32 = aVector.z;
	}

	template<typename T>
	inline void Matrix4x4<T>::SetRight(const Vector3<T>& aVector)
	{
		m11 = aVector.x;
		m21 = aVector.y;
		m31 = aVector.z;
	}

	template<typename T>
	inline void Matrix4x4<T>::Serialize(std::ostream& aStream) const
	{
		aStream.write(reinterpret_cast<const char*>(&m11), sizeof(m11) * 16);
	}

	template<typename T>
	inline void Matrix4x4<T>::Deserialize(std::istream& aStream)
	{
		aStream.read(reinterpret_cast<char*>(&m11), sizeof(m11) * 16);
	}

	template <class T>
	inline Vector3<T> Matrix4x4<T>::GetEulerAngles() const
	{
		if (CloseEnough(static_cast<float>((*this)(1, 3)), -1.0f)) {
			float x = 0;
			float y = Pi / 2;
			float z = x + atan2((*this)(2, 1), (*this)(3, 1));
			return { y, x, z };
		}
		else if (CloseEnough(static_cast<float>((*this)(1, 3)), 1.0f)) {
			float x = 0;
			float y = -Pi / 2;
			float z = -x + atan2(-(*this)(2, 1), -(*this)(3, 1));
			return { y, x, z };
		}
		else
		{
			float x1 = -asin((*this)(1, 3));
			float y1 = atan2((*this)(2, 3) / cos(x1), (*this)(3, 3) / cos(x1));
			float z1 = atan2((*this)(1, 2) / cos(x1), (*this)(1, 1) / cos(x1));

			float x2 = Pi - x1;
			float y2 = atan2((*this)(2, 3) / cos(x2), (*this)(3, 3) / cos(x2));
			float z2 = atan2((*this)(1, 2) / cos(x2), (*this)(1, 1) / cos(x2));

			if ((std::abs(x1) + std::abs(y1) + std::abs(z1)) <= (std::abs(x2) + std::abs(y2) + std::abs(z2)))
			{
				return { y1, x1, z1 };
			}
			else
			{
				return { y2, x2, z2 };
			}
		}
	}

	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator+=(const Matrix4x4<T>& aMatrix)
	{
		m11 += aMatrix.m11;
		m12 += aMatrix.m12;
		m13 += aMatrix.m13;
		m14 += aMatrix.m14;
		m21 += aMatrix.m21;
		m22 += aMatrix.m22;
		m23 += aMatrix.m23;
		m24 += aMatrix.m24;
		m31 += aMatrix.m31;
		m32 += aMatrix.m32;
		m33 += aMatrix.m33;
		m34 += aMatrix.m34;
		m41 += aMatrix.m41;
		m42 += aMatrix.m42;
		m43 += aMatrix.m43;
		m44 += aMatrix.m44;
		return *this;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::operator+(const Matrix4x4<T>& aMatrix) const
	{
		Matrix4x4<T> result = *this;
		return result += aMatrix;
	}

	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator+=(const T& aScalar)
	{
		m11 += aScalar;
		m12 += aScalar;
		m13 += aScalar;
		m14 += aScalar;
		m21 += aScalar;
		m22 += aScalar;
		m23 += aScalar;
		m24 += aScalar;
		m31 += aScalar;
		m32 += aScalar;
		m33 += aScalar;
		m34 += aScalar;
		m41 += aScalar;
		m42 += aScalar;
		m43 += aScalar;
		m44 += aScalar;
		return *this;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::operator+(const T& aScalar) const
	{
		Matrix4x4<T> result = *this;
		return result += aScalar;
	}

	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator-=(const Matrix4x4<T>& aMatrix)
	{
		m11 -= aMatrix.m11;
		m12 -= aMatrix.m12;
		m13 -= aMatrix.m13;
		m14 -= aMatrix.m14;
		m21 -= aMatrix.m21;
		m22 -= aMatrix.m22;
		m23 -= aMatrix.m23;
		m24 -= aMatrix.m24;
		m31 -= aMatrix.m31;
		m32 -= aMatrix.m32;
		m33 -= aMatrix.m33;
		m34 -= aMatrix.m34;
		m41 -= aMatrix.m41;
		m42 -= aMatrix.m42;
		m43 -= aMatrix.m43;
		m44 -= aMatrix.m44;
		return *this;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::operator-(const Matrix4x4<T>& aMatrix) const
	{
		Matrix4x4<T> result = *this;
		return result -= aMatrix;
	}

	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator-=(const T& aScalar)
	{
		m11 -= aScalar;
		m12 -= aScalar;
		m13 -= aScalar;
		m14 -= aScalar;
		m21 -= aScalar;
		m22 -= aScalar;
		m23 -= aScalar;
		m24 -= aScalar;
		m31 -= aScalar;
		m32 -= aScalar;
		m33 -= aScalar;
		m34 -= aScalar;
		m41 -= aScalar;
		m42 -= aScalar;
		m43 -= aScalar;
		m44 -= aScalar;
		return *this;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::operator-(const T& aScalar) const
	{
		Matrix4x4<T> result = *this;
		return result -= aScalar;
	}

	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator*=(const Matrix4x4<T>& aMatrix)
	{
		*this = (*this) * aMatrix;
		return *this;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::operator*(const Matrix4x4<T>& aMatrix) const
	{
		return Matrix4x4<T>(
			m11 * aMatrix.m11 + m12 * aMatrix.m21 + m13 * aMatrix.m31 + m14 * aMatrix.m41,
			m11 * aMatrix.m12 + m12 * aMatrix.m22 + m13 * aMatrix.m32 + m14 * aMatrix.m42,
			m11 * aMatrix.m13 + m12 * aMatrix.m23 + m13 * aMatrix.m33 + m14 * aMatrix.m43,
			m11 * aMatrix.m14 + m12 * aMatrix.m24 + m13 * aMatrix.m34 + m14 * aMatrix.m44,
			m21 * aMatrix.m11 + m22 * aMatrix.m21 + m23 * aMatrix.m31 + m24 * aMatrix.m41,
			m21 * aMatrix.m12 + m22 * aMatrix.m22 + m23 * aMatrix.m32 + m24 * aMatrix.m42,
			m21 * aMatrix.m13 + m22 * aMatrix.m23 + m23 * aMatrix.m33 + m24 * aMatrix.m43,
			m21 * aMatrix.m14 + m22 * aMatrix.m24 + m23 * aMatrix.m34 + m24 * aMatrix.m44,
			m31 * aMatrix.m11 + m32 * aMatrix.m21 + m33 * aMatrix.m31 + m34 * aMatrix.m41,
			m31 * aMatrix.m12 + m32 * aMatrix.m22 + m33 * aMatrix.m32 + m34 * aMatrix.m42,
			m31 * aMatrix.m13 + m32 * aMatrix.m23 + m33 * aMatrix.m33 + m34 * aMatrix.m43,
			m31 * aMatrix.m14 + m32 * aMatrix.m24 + m33 * aMatrix.m34 + m34 * aMatrix.m44,
			m41 * aMatrix.m11 + m42 * aMatrix.m21 + m43 * aMatrix.m31 + m44 * aMatrix.m41,
			m41 * aMatrix.m12 + m42 * aMatrix.m22 + m43 * aMatrix.m32 + m44 * aMatrix.m42,
			m41 * aMatrix.m13 + m42 * aMatrix.m23 + m43 * aMatrix.m33 + m44 * aMatrix.m43,
			m41 * aMatrix.m14 + m42 * aMatrix.m24 + m43 * aMatrix.m34 + m44 * aMatrix.m44);
	}

	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator*=(const T& aScalar)
	{
		m11 *= aScalar;
		m12 *= aScalar;
		m13 *= aScalar;
		m14 *= aScalar;
		m21 *= aScalar;
		m22 *= aScalar;
		m23 *= aScalar;
		m24 *= aScalar;
		m31 *= aScalar;
		m32 *= aScalar;
		m33 *= aScalar;
		m34 *= aScalar;
		m41 *= aScalar;
		m42 *= aScalar;
		m43 *= aScalar;
		m44 *= aScalar;
		return *this;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::operator*(const T& aScalar) const
	{
		Matrix4x4<T> result = *this;
		return result *= aScalar;
	}

	template<typename T>
	inline Vector4<T> Matrix4x4<T>::operator*(const Vector4<T>& aRowVector) const
	{
		Vector4<T> result;
		result.x = (m11 * aRowVector.x) + (m21 * aRowVector.y) + (m31 * aRowVector.z) + (m41 * aRowVector.w);
		result.y = (m12 * aRowVector.x) + (m22 * aRowVector.y) + (m32 * aRowVector.z) + (m42 * aRowVector.w);
		result.z = (m13 * aRowVector.x) + (m23 * aRowVector.y) + (m33 * aRowVector.z) + (m43 * aRowVector.w);
		result.w = (m14 * aRowVector.x) + (m24 * aRowVector.y) + (m34 * aRowVector.z) + (m44 * aRowVector.w);
		return result;
	}

	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator/=(const T& aScalar)
	{
		assert(aScalar != 0);
		if (aScalar == 0)
		{
			return *this;
		}
		double invert = 1. / static_cast<double>(aScalar);
		m11 *= invert;
		m12 *= invert;
		m13 *= invert;
		m14 *= invert;
		m21 *= invert;
		m22 *= invert;
		m23 *= invert;
		m24 *= invert;
		m31 *= invert;
		m32 *= invert;
		m33 *= invert;
		m34 *= invert;
		m41 *= invert;
		m42 *= invert;
		m43 *= invert;
		m44 *= invert;
		return *this;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::operator/(const T& aScalar) const
	{
		Matrix4x4<T> result = *this;
		return result /= aScalar;
	}

	template<typename T>
	inline bool Matrix4x4<T>::operator==(const Matrix4x4<T>& aMatrix) const
	{
		return m11 == aMatrix.m11
			&& m12 == aMatrix.m12
			&& m13 == aMatrix.m13
			&& m14 == aMatrix.m14
			&& m21 == aMatrix.m21
			&& m22 == aMatrix.m22
			&& m23 == aMatrix.m23
			&& m24 == aMatrix.m24
			&& m31 == aMatrix.m31
			&& m32 == aMatrix.m32
			&& m33 == aMatrix.m33
			&& m34 == aMatrix.m34
			&& m41 == aMatrix.m41
			&& m42 == aMatrix.m42
			&& m43 == aMatrix.m43
			&& m44 == aMatrix.m44;
	}

	template<>
	inline bool Matrix4x4<float>::operator==(const Matrix4x4<float>& aMatrix) const
	{
		return std::abs(m11 - aMatrix.m11) < FloatTolerance
			&& std::abs(m12 - aMatrix.m12) < FloatTolerance
			&& std::abs(m13 - aMatrix.m13) < FloatTolerance
			&& std::abs(m14 - aMatrix.m14) < FloatTolerance
			&& std::abs(m21 - aMatrix.m21) < FloatTolerance
			&& std::abs(m22 - aMatrix.m22) < FloatTolerance
			&& std::abs(m23 - aMatrix.m23) < FloatTolerance
			&& std::abs(m24 - aMatrix.m24) < FloatTolerance
			&& std::abs(m31 - aMatrix.m31) < FloatTolerance
			&& std::abs(m32 - aMatrix.m32) < FloatTolerance
			&& std::abs(m33 - aMatrix.m33) < FloatTolerance
			&& std::abs(m34 - aMatrix.m34) < FloatTolerance
			&& std::abs(m41 - aMatrix.m41) < FloatTolerance
			&& std::abs(m42 - aMatrix.m42) < FloatTolerance
			&& std::abs(m43 - aMatrix.m43) < FloatTolerance
			&& std::abs(m44 - aMatrix.m44) < FloatTolerance;
	}

	template<>
	inline bool Matrix4x4<double>::operator==(const Matrix4x4<double>& aMatrix) const
	{
		return std::abs(m11 - aMatrix.m11) < DoubleTolerance
			&& std::abs(m12 - aMatrix.m12) < DoubleTolerance
			&& std::abs(m13 - aMatrix.m13) < DoubleTolerance
			&& std::abs(m14 - aMatrix.m14) < DoubleTolerance
			&& std::abs(m21 - aMatrix.m21) < DoubleTolerance
			&& std::abs(m22 - aMatrix.m22) < DoubleTolerance
			&& std::abs(m23 - aMatrix.m23) < DoubleTolerance
			&& std::abs(m24 - aMatrix.m24) < DoubleTolerance
			&& std::abs(m31 - aMatrix.m31) < DoubleTolerance
			&& std::abs(m32 - aMatrix.m32) < DoubleTolerance
			&& std::abs(m33 - aMatrix.m33) < DoubleTolerance
			&& std::abs(m34 - aMatrix.m34) < DoubleTolerance
			&& std::abs(m41 - aMatrix.m41) < DoubleTolerance
			&& std::abs(m42 - aMatrix.m42) < DoubleTolerance
			&& std::abs(m43 - aMatrix.m43) < DoubleTolerance
			&& std::abs(m44 - aMatrix.m44) < DoubleTolerance;
	}

	template<typename T>
	inline bool Matrix4x4<T>::operator!=(const Matrix4x4<T>& aMatrix) const
	{
		return m11 != aMatrix.m11
			&& m12 != aMatrix.m12
			&& m13 != aMatrix.m13
			&& m14 != aMatrix.m14
			&& m21 != aMatrix.m21
			&& m22 != aMatrix.m22
			&& m23 != aMatrix.m23
			&& m24 != aMatrix.m24
			&& m31 != aMatrix.m31
			&& m32 != aMatrix.m32
			&& m33 != aMatrix.m33
			&& m34 != aMatrix.m34
			&& m41 != aMatrix.m41
			&& m42 != aMatrix.m42
			&& m43 != aMatrix.m43
			&& m44 != aMatrix.m44;
	}

	template<>
	inline bool Matrix4x4<float>::operator!=(const Matrix4x4<float>& aMatrix) const
	{
		return !(*this == aMatrix);
	}

	template<>
	inline bool Matrix4x4<double>::operator!=(const Matrix4x4<double>& aMatrix) const
	{
		return !(*this == aMatrix);
	}
}