#pragma once
#include "Matrix4x4.hpp"

namespace Crimson
{
	template <typename T>
	class Matrix3x3
	{
	public:
		T m11;
		T m12;
		T m13;
		T m21;
		T m22;
		T m23;
		T m31;
		T m32;
		T m33;

		static const Matrix3x3 Identity;

		// Creates the identity matrix.
		Matrix3x3<T>();
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix) = default;
		Matrix3x3<T>(Matrix3x3<T>&& aMatrix) = default;
		// Copies the top left 3x3 part of the Matrix4x4.
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);
		Matrix3x3<T>(T aM11, T aM12, T aM13, T aM21, T aM22, T aM23, T aM31, T aM32, T aM33);
		Matrix3x3<T>(const Vector3<T>& aFirstRow, const Vector3<T>& aSecondRow = Vector3<T>(T(), T(1), T()), const Vector3<T>& aThirdRow = Vector3<T>(T(), T(), T(1)));
		~Matrix3x3() = default;

		// Inserts the 3x3 part in the top left of the identity Matrix4x4
		operator Matrix4x4<T>() const;
		inline Matrix3x3<T>& operator=(const Matrix3x3<T>& aMatrix) = default;
		inline Matrix3x3<T>& operator=(Matrix3x3<T>&& aMatrix) = default;

		const T& operator()(const unsigned int aRow, const unsigned int aColumn) const;
		T& operator()(const unsigned int aRow, const unsigned int aColumn);

		static Matrix3x3<T> CreateRotationAroundX(const T& anAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(const T& anAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(const T& anAngleInRadians);

		static Matrix3x3<T> CreateScaleMatrix(const Vector3<T>& aScale);
		static Matrix3x3<T> CreateRotationMatrix(const Vector3<T>& aRotation);

		// Assumes aTransform is made up of nothing but rotations and translations.
		inline Matrix3x3<T> GetInverse2D() const;
		inline Matrix3x3<T> GetTranspose() const;

		inline Matrix3x3<T> GetMatrixUnscaled() const;

		// Returns the Matrix Angles in Radians
		inline Vector3<T> GetRotation() const;
		inline Vector3<T> GetScale() const;

		inline Vector3<T> GetForward() const;
		inline Vector3<T> GetUp() const;
		inline Vector3<T> GetRight() const;

		inline void SetForward(const Vector3<T>& aVector);
		inline void SetUp(const Vector3<T>& aVector);
		inline void SetRight(const Vector3<T>& aVector);

		Vector3<T> GetEulerAngles() const;

		void Serialize(std::ostream& aStream) const;
		void Deserialize(std::istream& aStream);

		inline Matrix3x3<T>& operator+=(const Matrix3x3<T>& aMatrix);
		inline Matrix3x3<T> operator+(const Matrix3x3<T>& aMatrix) const;
		inline Matrix3x3<T>& operator+=(const T& aScalar);
		inline Matrix3x3<T> operator+(const T& aScalar) const;

		inline Matrix3x3<T>& operator-=(const Matrix3x3<T>& aMatrix);
		inline Matrix3x3<T> operator-(const Matrix3x3<T>& aMatrix) const;
		inline Matrix3x3<T>& operator-=(const T& aScalar);
		inline Matrix3x3<T> operator-(const T& aScalar) const;

		inline Matrix3x3<T>& operator*= (const Matrix3x3<T>& aMatrix);
		inline Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix) const;
		inline Matrix3x3<T>& operator*=(const T& aScalar);
		inline Matrix3x3<T> operator*(const T& aScalar) const;

		inline Vector3<T> operator*(const Vector3<T>& aRowVector) const;

		inline Matrix3x3<T>& operator/=(const T& aScalar);
		inline Matrix3x3<T> operator/(const T& aScalar) const;

		inline bool operator==(const Matrix3x3<T>& aMatrix) const;
		inline bool operator!=(const Matrix3x3<T>& aMatrix) const;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename T> const Matrix3x3<T> Matrix3x3<T>::Identity{};

	typedef Matrix3x3<float> Matrix3x3f;
	typedef Matrix3x3<int>  Matrix3x3i;
	typedef Matrix3x3<float> Mat3;

	template<typename T>
	Vector3<T>& operator*=(Vector3<T>& aRowVector, const Matrix3x3<T>& aMatrix) {
		return aRowVector = aMatrix * aRowVector;
	}
	template<typename T>
	Vector3<T> operator*(const Vector3<T>& aRowVector, const Matrix3x3<T>& aMatrix) {
		return aMatrix * aRowVector;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3() :
		m11(T(1)), m12(T()), m13(T()),
		m21(T()), m22(T(1)), m23(T()),
		m31(T()), m32(T()), m33(T(1))
	{
	}

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix) :
		m11(aMatrix.m11), m12(aMatrix.m12), m13(aMatrix.m13),
		m21(aMatrix.m21), m22(aMatrix.m22), m23(aMatrix.m23),
		m31(aMatrix.m31), m32(aMatrix.m32), m33(aMatrix.m33)
	{
	}

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3(T a11, T a12, T a13, T a21, T a22, T a23, T a31, T a32, T a33) :
		m11(a11), m12(a12), m13(a13),
		m21(a21), m22(a22), m23(a23),
		m31(a31), m32(a32), m33(a33)
	{
	}

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3(const Vector3<T>& aFirstRow, const Vector3<T>& aSecondRow, const Vector3<T>& aThirdRow) :
		m11(aFirstRow.x), m12(aFirstRow.y), m13(aFirstRow.z),
		m21(aSecondRow.x), m22(aSecondRow.y), m23(aSecondRow.z),
		m31(aThirdRow.x), m32(aThirdRow.y), m33(aThirdRow.z)
	{
	}

	template<typename T>
	inline Matrix3x3<T>::operator Matrix4x4<T>() const
	{
		return {
			m11, m21, m31,T(),
			m12, m22, m32,T(),
			m13, m23, m33,T(),
			T(),T(),T(),T(1) };
	}

	template<typename T>
	inline const T& Matrix3x3<T>::operator()(const unsigned int aRow, const unsigned int aColumn) const
	{
		return *((&m11) + ((aRow - 1) * 3) + (aColumn - 1));
	}

	template<typename T>
	inline T& Matrix3x3<T>::operator()(const unsigned int aRow, const unsigned int aColumn)
	{
		return *((&m11) + ((aRow - 1) * 3) + (aColumn - 1));
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(const T& anAngleInRadians)
	{
		return Matrix3x3<T>{
			{ T(1), T(), T() },
			{ T(), T(cos(anAngleInRadians)), T(sin(anAngleInRadians)) },
			{ T(), T(-sin(anAngleInRadians)), T(cos(anAngleInRadians)) } };
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(const T& anAngleInRadians)
	{
		return Matrix3x3<T>{
			{ T(cos(anAngleInRadians)), T(), T(-sin(anAngleInRadians)) },
			{ T(), T(1), T() },
			{ T(sin(anAngleInRadians)), T(), T(cos(anAngleInRadians)) } };
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(const T& anAngleInRadians)
	{
		return Matrix3x3<T>{
			{ T(cos(anAngleInRadians)), T(sin(anAngleInRadians)), T()},
			{ T(-sin(anAngleInRadians)), T(cos(anAngleInRadians)), T() },
			{ T(), T(), T(1) } };
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateScaleMatrix(const Vector3<T>& aScale)
	{
		return Matrix3x3<T>{
			{ T(aScale.x), T(), T() },
			{ T(), T(aScale.y), T() },
			{ T(), T(), T(aScale.z) }
		};
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationMatrix(const Vector3<T>& aRotation)
	{
		return CreateRotationAroundX(aRotation.x) * CreateRotationAroundY(aRotation.y) * CreateRotationAroundZ(aRotation.z);
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::GetInverse2D() const
	{
		Matrix3x3<T> result{
			m11, m21, m13,
			m12, m22, m23,
			T(), T(), T(1) };
		Vector3<T> translate(-m31, -m32, T(1));
		translate = translate * result;
		result.m31 = translate.x;
		result.m32 = translate.y;
		return result;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::GetTranspose() const
	{
		return {
			m11, m21, m31,
			m12, m22, m32,
			m13, m23, m33,
		};
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::GetMatrixUnscaled() const
	{
		Vector3<float> scale = GetScale();

		Matrix3x3<T> resultMat = *this;

		Vector3<T> scaleDividers(1 / scale.x, 1 / scale.y, 1 / scale.z);
		resultMat.m11 *= scaleDividers.x;
		resultMat.m12 *= scaleDividers.x;
		resultMat.m13 *= scaleDividers.x;

		resultMat.m21 *= scaleDividers.y;
		resultMat.m22 *= scaleDividers.y;
		resultMat.m23 *= scaleDividers.y;

		resultMat.m31 *= scaleDividers.z;
		resultMat.m32 *= scaleDividers.z;
		resultMat.m33 *= scaleDividers.z;

		return resultMat;
	}

	template<typename T>
	inline Vector3<T> Matrix3x3<T>::GetRotation() const
	{
		//https://stackoverflow.com/a/18436193
		// X and Y flipped for some reason, probably different rotational order

		if (GimbalCheck(static_cast<float>((*this)(1, 3)), -1.0f))
		{
			float x = 0; //gimbal lock, value of x doesn't matter
			float y = PiHalf;
			float z = x + atan2((*this)(2, 1), (*this)(3, 1));
			return { y, x, z };
		}
		else if (GimbalCheck(static_cast<float>((*this)(1, 3)), 1.0f))
		{
			float x = 0;
			float y = -PiHalf;
			float z = -x + atan2(-(*this)(2, 1), -(*this)(3, 1));
			return { y, x, z };
		}
		else
		{	//two solutions exist
			float x1 = -asin((*this)(1, 3));
			float y1 = atan2((*this)(2, 3) / cos(x1), (*this)(3, 3) / cos(x1));
			float z1 = atan2((*this)(1, 2) / cos(x1), (*this)(1, 1) / cos(x1));

			float x2 = Pi - x1;
			float y2 = atan2((*this)(2, 3) / cos(x2), (*this)(3, 3) / cos(x2));
			float z2 = atan2((*this)(1, 2) / cos(x2), (*this)(1, 1) / cos(x2));

			//choose one solution to return
			//for example the "shortest" rotation
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
	inline Vector3<T> Matrix3x3<T>::GetScale() const
	{
		return Vector3<T>(Vector3<T>(m11, m12, m13).Length(), Vector3<T>(m21, m22, m23).Length(), Vector3<T>(m31, m32, m33).Length());
	}

	template<typename T>
	inline Vector3<T> Matrix3x3<T>::GetForward() const
	{
		return Vector3<T>(m13, m23, m33).GetNormalized();
	}

	template<typename T>
	inline Vector3<T> Matrix3x3<T>::GetUp() const
	{
		return Vector3<T>(m12, m22, m32).GetNormalized();
	}

	template<typename T>
	inline Vector3<T> Matrix3x3<T>::GetRight() const
	{
		return Vector3<T>(m11, m21, m31).GetNormalized();
	}

	template<typename T>
	inline void Matrix3x3<T>::SetForward(const Vector3<T>& aVector)
	{
		m13 = aVector.x;
		m23 = aVector.y;
		m33 = aVector.z;
	}

	template<typename T>
	inline void Matrix3x3<T>::SetUp(const Vector3<T>& aVector)
	{
		m12 = aVector.x;
		m22 = aVector.y;
		m32 = aVector.z;
	}

	template<typename T>
	inline void Matrix3x3<T>::SetRight(const Vector3<T>& aVector)
	{
		m11 = aVector.x;
		m21 = aVector.y;
		m31 = aVector.z;
	}

	template <class T>
	inline Vector3<T> Matrix3x3<T>::GetEulerAngles() const
	{
		if (GimbalCheck(static_cast<float>((*this)(1, 3)), -1.0f)) {
			float x = 0;
			float y = Pi / 2;
			float z = x + atan2((*this)(2, 1), (*this)(3, 1));
			return { y, x, z };
		}
		else if (GimbalCheck(static_cast<float>((*this)(1, 3)), 1.0f)) {
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
	inline void Matrix3x3<T>::Serialize(std::ostream& aStream) const
	{
		aStream.write(reinterpret_cast<const char*>(&m11), sizeof(m11) * 9);
	}

	template<typename T>
	inline void Matrix3x3<T>::Deserialize(std::istream& aStream)
	{
		aStream.read(reinterpret_cast<char*>(&m11), sizeof(m11) * 9);
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator+=(const Matrix3x3<T>& aMatrix)
	{
		m11 += aMatrix.m11;
		m12 += aMatrix.m12;
		m13 += aMatrix.m13;
		m21 += aMatrix.m21;
		m22 += aMatrix.m22;
		m23 += aMatrix.m23;
		m31 += aMatrix.m31;
		m32 += aMatrix.m32;
		m33 += aMatrix.m33;
		return *this;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::operator+(const Matrix3x3<T>& aMatrix) const
	{
		Matrix3x3<T> result = *this;
		return result += aMatrix;
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator+=(const T& aScalar)
	{
		m11 += aScalar;
		m12 += aScalar;
		m13 += aScalar;
		m21 += aScalar;
		m22 += aScalar;
		m23 += aScalar;
		m31 += aScalar;
		m32 += aScalar;
		m33 += aScalar;
		return *this;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::operator+(const T& aScalar) const
	{
		Matrix3x3<T> result = *this;
		return result += aScalar;
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator-=(const Matrix3x3<T>& aMatrix)
	{
		m11 -= aMatrix.m11;
		m12 -= aMatrix.m12;
		m13 -= aMatrix.m13;
		m21 -= aMatrix.m21;
		m22 -= aMatrix.m22;
		m23 -= aMatrix.m23;
		m31 -= aMatrix.m31;
		m32 -= aMatrix.m32;
		m33 -= aMatrix.m33;
		return *this;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::operator-(const Matrix3x3<T>& aMatrix) const
	{
		Matrix3x3<T> result = *this;
		return result -= aMatrix;
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator-=(const T& aScalar)
	{
		m11 -= aScalar;
		m12 -= aScalar;
		m13 -= aScalar;
		m21 -= aScalar;
		m22 -= aScalar;
		m23 -= aScalar;
		m31 -= aScalar;
		m32 -= aScalar;
		m33 -= aScalar;
		return *this;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::operator-(const T& aScalar) const
	{
		Matrix3x3<T> result = *this;
		return result -= aScalar;
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator*=(const Matrix3x3<T>& aMatrix)
	{
		*this = (*this) * aMatrix;
		return *this;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::operator*(const Matrix3x3<T>& aMatrix) const
	{
		return Matrix3x3<T>(
			m11 * aMatrix.m11 + m12 * aMatrix.m21 + m13 * aMatrix.m31,
			m11 * aMatrix.m12 + m12 * aMatrix.m22 + m13 * aMatrix.m32,
			m11 * aMatrix.m13 + m12 * aMatrix.m23 + m13 * aMatrix.m33,
			m21 * aMatrix.m11 + m22 * aMatrix.m21 + m23 * aMatrix.m31,
			m21 * aMatrix.m12 + m22 * aMatrix.m22 + m23 * aMatrix.m32,
			m21 * aMatrix.m13 + m22 * aMatrix.m23 + m23 * aMatrix.m33,
			m31 * aMatrix.m11 + m32 * aMatrix.m21 + m33 * aMatrix.m31,
			m31 * aMatrix.m12 + m32 * aMatrix.m22 + m33 * aMatrix.m32,
			m31 * aMatrix.m13 + m32 * aMatrix.m23 + m33 * aMatrix.m33);
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator*=(const T& aScalar)
	{
		m11 *= aScalar;
		m12 *= aScalar;
		m13 *= aScalar;
		m21 *= aScalar;
		m22 *= aScalar;
		m23 *= aScalar;
		m31 *= aScalar;
		m32 *= aScalar;
		m33 *= aScalar;
		return *this;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::operator*(const T& aScalar) const
	{
		Matrix3x3<T> result = *this;
		return result *= aScalar;
	}

	template<typename T>
	inline Vector3<T> Matrix3x3<T>::operator*(const Vector3<T>& aRowVector) const
	{
		Vector3<T> result;
		result.x = (m11 * aRowVector.x) + (m21 * aRowVector.y) + (m31 * aRowVector.z);
		result.y = (m12 * aRowVector.x) + (m22 * aRowVector.y) + (m32 * aRowVector.z);
		result.z = (m13 * aRowVector.x) + (m23 * aRowVector.y) + (m33 * aRowVector.z);
		return result;
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator/=(const T& aScalar)
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
		m21 *= invert;
		m22 *= invert;
		m23 *= invert;
		m31 *= invert;
		m32 *= invert;
		m33 *= invert;
		return *this;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::operator/(const T& aScalar) const
	{
		Matrix3x3<T> result = *this;
		return result /= aScalar;
	}

	template<typename T>
	inline bool Matrix3x3<T>::operator==(const Matrix3x3<T>& aMatrix) const
	{
		return m11 == aMatrix.m11
			&& m12 == aMatrix.m12
			&& m13 == aMatrix.m13
			&& m21 == aMatrix.m21
			&& m22 == aMatrix.m22
			&& m23 == aMatrix.m23
			&& m31 == aMatrix.m31
			&& m32 == aMatrix.m32
			&& m33 == aMatrix.m33;
	}

	template<>
	inline bool Matrix3x3<float>::operator==(const Matrix3x3<float>& aMatrix) const
	{
		return std::abs(m11 - aMatrix.m11) < FloatTolerance
			&& std::abs(m12 - aMatrix.m12) < FloatTolerance
			&& std::abs(m13 - aMatrix.m13) < FloatTolerance
			&& std::abs(m21 - aMatrix.m21) < FloatTolerance
			&& std::abs(m22 - aMatrix.m22) < FloatTolerance
			&& std::abs(m23 - aMatrix.m23) < FloatTolerance
			&& std::abs(m31 - aMatrix.m31) < FloatTolerance
			&& std::abs(m32 - aMatrix.m32) < FloatTolerance
			&& std::abs(m33 - aMatrix.m33) < FloatTolerance;
	}

	template<>
	inline bool Matrix3x3<double>::operator==(const Matrix3x3<double>& aMatrix) const
	{
		return std::abs(m11 - aMatrix.m11) < DoubleTolerance
			&& std::abs(m12 - aMatrix.m12) < DoubleTolerance
			&& std::abs(m13 - aMatrix.m13) < DoubleTolerance
			&& std::abs(m21 - aMatrix.m21) < DoubleTolerance
			&& std::abs(m22 - aMatrix.m22) < DoubleTolerance
			&& std::abs(m23 - aMatrix.m23) < DoubleTolerance
			&& std::abs(m31 - aMatrix.m31) < DoubleTolerance
			&& std::abs(m32 - aMatrix.m32) < DoubleTolerance
			&& std::abs(m33 - aMatrix.m33) < DoubleTolerance;
	}

	template<typename T>
	inline bool Matrix3x3<T>::operator!=(const Matrix3x3<T>& aMatrix) const
	{
		return m11 != aMatrix.m11
			&& m12 != aMatrix.m12
			&& m13 != aMatrix.m13
			&& m21 != aMatrix.m21
			&& m22 != aMatrix.m22
			&& m23 != aMatrix.m23
			&& m31 != aMatrix.m31
			&& m32 != aMatrix.m32
			&& m33 != aMatrix.m33;
	}

	template<>
	inline bool Matrix3x3<float>::operator!=(const Matrix3x3<float>& aMatrix) const
	{
		return !(*this == aMatrix);
	}

	template<>
	inline bool Matrix3x3<double>::operator!=(const Matrix3x3<double>& aMatrix) const
	{
		return !(*this == aMatrix);
	}
}