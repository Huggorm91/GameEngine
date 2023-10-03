#pragma once
#include "Vector4.hpp"
#include "UtilityFunctions.hpp"
#include <array>
#include <math.h>

namespace Crimson
{
	template <typename T>
	class Matrix4x4
	{
	public:
		static const Matrix4x4<T> Null;

		// Creates the identity matrix.
		Matrix4x4<T>();
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix) = default;
		Matrix4x4<T>(Matrix4x4<T>&& aMatrix) = default;
		Matrix4x4<T>(std::array<std::array<T, 4>, 4> anArray);
		Matrix4x4<T>(const Vector4<T>& aFirstRow, const Vector4<T>& aSecondRow = Vector4<T>(T(), T(1), T(), T()), const Vector4<T>& aThirdRow = Vector4<T>(T(), T(), T(1), T()), const Vector4<T>& aFourthRow = Vector4<T>(T(), T(), T(), T(1)));
		~Matrix4x4() = default;

		inline Matrix4x4<T>& operator=(const Matrix4x4<T>& aMatrix) = default;
		inline Matrix4x4<T>& operator=(Matrix4x4<T>&& aMatrix) = default;

		T& operator()(const unsigned int aRow, const unsigned int aColumn);
		const T& operator()(const unsigned int aRow, const unsigned int aColumn) const;

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
		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform);
		inline Matrix4x4<T> GetFastInverse() const;
		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);
		inline Matrix4x4<T> Transpose() const;

		inline Vector3<T> GetForward() const;
		inline Vector3<T> GetUp() const;
		inline Vector3<T> GetRight() const;

		inline void SetForward(const Vector3<T>& aVector);
		inline void SetUp(const Vector3<T>& aVector);
		inline void SetRight(const Vector3<T>& aVector);

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

	private:
		std::array<std::array<T, 4>, 4> myValues;
	};

	template<typename T> const Matrix4x4<T> Matrix4x4<T>::Null{};

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
	inline Matrix4x4<T>::Matrix4x4() : myValues{ {{ T(1), T(), T(), T() }, { T(), T(1), T(), T() }, { T(), T(), T(1), T() }, { T(), T(), T(), T(1) }} }
	{
	}

	template<typename T>
	inline Matrix4x4<T>::Matrix4x4(std::array<std::array<T, 4>, 4> anArray) : myValues(anArray)
	{
	}

	template<typename T>
	inline Matrix4x4<T>::Matrix4x4(const Vector4<T>& aFirstRow, const Vector4<T>& aSecondRow, const Vector4<T>& aThirdRow, const Vector4<T>& aFourthRow) : myValues{ {
		{aFirstRow.x, aFirstRow.y, aFirstRow.z, aFirstRow.w},
		{aSecondRow.x, aSecondRow.y, aSecondRow.z, aSecondRow.w},
		{aThirdRow.x, aThirdRow.y, aThirdRow.z, aThirdRow.w},
		{aFourthRow.x, aFourthRow.y, aFourthRow.z, aFourthRow.w}} }
	{
	}

	template<typename T>
	inline T& Matrix4x4<T>::operator()(const unsigned int aRow, const unsigned int aColumn)
	{
		assert(aRow != 0 && aColumn != 0);
		return myValues[aRow - 1][aColumn - 1];
	}

	template<typename T>
	inline const T& Matrix4x4<T>::operator()(const unsigned int aRow, const unsigned int aColumn) const
	{
		assert(aRow != 0 && aColumn != 0);
		return myValues[aRow - 1][aColumn - 1];
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
	inline Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
	{
		Matrix4x4<T> transpose = aMatrixToTranspose;
		Swap(transpose(1, 2), transpose(2, 1));
		Swap(transpose(1, 3), transpose(3, 1));
		Swap(transpose(1, 4), transpose(4, 1));
		Swap(transpose(2, 3), transpose(3, 2));
		Swap(transpose(2, 4), transpose(4, 2));
		Swap(transpose(3, 4), transpose(4, 3));
		return transpose;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::Transpose() const
	{
		return Transpose(*this);
	}

	template<typename T>
	inline Vector3<T> Matrix4x4<T>::GetForward() const
	{
		return Vector3<T>(myValues[0][2], myValues[1][2], myValues[2][2]).GetNormalized();
	}

	template<typename T>
	inline Vector3<T> Matrix4x4<T>::GetUp() const
	{
		return Vector3<T>(myValues[0][1], myValues[1][1], myValues[2][1]).GetNormalized();
	}

	template<typename T>
	inline Vector3<T> Matrix4x4<T>::GetRight() const
	{
		return Vector3<T>(myValues[0][0], myValues[1][0], myValues[2][0]).GetNormalized();
	}

	template<typename T>
	inline void Matrix4x4<T>::SetForward(const Vector3<T>& aVector)
	{
		myValues[0][2] = aVector.x;
		myValues[1][2] = aVector.y;
		myValues[2][2] = aVector.z;
	}

	template<typename T>
	inline void Matrix4x4<T>::SetUp(const Vector3<T>& aVector)
	{
		myValues[0][1] = aVector.x;
		myValues[1][1] = aVector.y;
		myValues[2][1] = aVector.z;
	}

	template<typename T>
	inline void Matrix4x4<T>::SetRight(const Vector3<T>& aVector)
	{
		myValues[0][0] = aVector.x;
		myValues[1][0] = aVector.y;
		myValues[2][0] = aVector.z;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aTransform)
	{
		Matrix4x4<T> result{
			{ aTransform(1, 1), aTransform(2, 1), aTransform(3, 1), aTransform(1, 4)},
			{ aTransform(1,2), aTransform(2,2), aTransform(3,2), aTransform(2,4) },
			{ aTransform(1,3), aTransform(2,3), aTransform(3,3), aTransform(3,4) },
			{ T(),T(),T(),T(1) }};
		Vector4<T> translate(-aTransform(4, 1), -aTransform(4, 2), -aTransform(4, 3), aTransform(4, 4));
		translate = translate * result;
		result(4, 1) = translate.x;
		result(4, 2) = translate.y;
		result(4, 3) = translate.z;
		return result;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::GetFastInverse() const
	{
		return GetFastInverse(*this);
	}

	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator+=(const Matrix4x4<T>& aMatrix)
	{
		for (size_t row = 0; row < myValues.size(); row++)
		{
			for (size_t column = 0; column < myValues[0].size(); column++)
			{
				myValues[row][column] += aMatrix.myValues[row][column];
			}
		}
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
		for (size_t row = 0; row < myValues.size(); row++)
		{
			for (size_t column = 0; column < myValues[0].size(); column++)
			{
				myValues[row][column] += aScalar;
			}
		}
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
		for (size_t row = 0; row < myValues.size(); row++)
		{
			for (size_t column = 0; column < myValues[0].size(); column++)
			{
				myValues[row][column] -= aMatrix.myValues[row][column];
			}
		}
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
		for (size_t row = 0; row < myValues.size(); row++)
		{
			for (size_t column = 0; column < myValues[0].size(); column++)
			{
				myValues[row][column] -= aScalar;
			}
		}
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
		Vector4<T> firstColumn(aMatrix.myValues[0][0], aMatrix.myValues[1][0], aMatrix.myValues[2][0], aMatrix.myValues[3][0]);
		Vector4<T> secondColumn(aMatrix.myValues[0][1], aMatrix.myValues[1][1], aMatrix.myValues[2][1], aMatrix.myValues[3][1]);
		Vector4<T> thirdColumn(aMatrix.myValues[0][2], aMatrix.myValues[1][2], aMatrix.myValues[2][2], aMatrix.myValues[3][2]);
		Vector4<T> fourthColumn(aMatrix.myValues[0][3], aMatrix.myValues[1][3], aMatrix.myValues[2][3], aMatrix.myValues[3][3]);

		Vector4<T> firstRow(myValues[0][0], myValues[0][1], myValues[0][2], myValues[0][3]);
		Vector4<T> secondRow(myValues[1][0], myValues[1][1], myValues[1][2], myValues[1][3]);
		Vector4<T> thirdRow(myValues[2][0], myValues[2][1], myValues[2][2], myValues[2][3]);
		Vector4<T> fourthRow(myValues[3][0], myValues[3][1], myValues[3][2], myValues[3][3]);

		myValues[0][0] = firstRow.Dot(firstColumn);
		myValues[0][1] = firstRow.Dot(secondColumn);
		myValues[0][2] = firstRow.Dot(thirdColumn);
		myValues[0][3] = firstRow.Dot(fourthColumn);

		myValues[1][0] = secondRow.Dot(firstColumn);
		myValues[1][1] = secondRow.Dot(secondColumn);
		myValues[1][2] = secondRow.Dot(thirdColumn);
		myValues[1][3] = secondRow.Dot(fourthColumn);

		myValues[2][0] = thirdRow.Dot(firstColumn);
		myValues[2][1] = thirdRow.Dot(secondColumn);
		myValues[2][2] = thirdRow.Dot(thirdColumn);
		myValues[2][3] = thirdRow.Dot(fourthColumn);

		myValues[3][0] = fourthRow.Dot(firstColumn);
		myValues[3][1] = fourthRow.Dot(secondColumn);
		myValues[3][2] = fourthRow.Dot(thirdColumn);
		myValues[3][3] = fourthRow.Dot(fourthColumn);
		return *this;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::operator*(const Matrix4x4<T>& aMatrix) const
	{
		Matrix4x4<T> result = *this;
		return result *= aMatrix;
	}

	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator*=(const T& aScalar)
	{
		for (size_t row = 0; row < myValues.size(); row++)
		{
			for (size_t column = 0; column < myValues[0].size(); column++)
			{
				myValues[row][column] *= aScalar;
			}
		}
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
		result.x = (myValues[0][0] * aRowVector.x) + (myValues[1][0] * aRowVector.y) + (myValues[2][0] * aRowVector.z) + (myValues[3][0] * aRowVector.w);
		result.y = (myValues[0][1] * aRowVector.x) + (myValues[1][1] * aRowVector.y) + (myValues[2][1] * aRowVector.z) + (myValues[3][1] * aRowVector.w);
		result.z = (myValues[0][2] * aRowVector.x) + (myValues[1][2] * aRowVector.y) + (myValues[2][2] * aRowVector.z) + (myValues[3][2] * aRowVector.w);
		result.w = (myValues[0][3] * aRowVector.x) + (myValues[1][3] * aRowVector.y) + (myValues[2][3] * aRowVector.z) + (myValues[3][3] * aRowVector.w);
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
		for (size_t row = 0; row < myValues.size(); row++)
		{
			for (size_t column = 0; column < myValues[0].size(); column++)
			{
				myValues[row][column] = static_cast<T>(invert * myValues[row][column]);
			}
		}
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
		for (size_t row = 0; row < myValues.size(); row++)
		{
			for (size_t column = 0; column < myValues[0].size(); column++)
			{
				if (myValues[row][column] != aMatrix.myValues[row][column])
				{
					return false;
				}
			}
		}
		return true;
	}

	template<typename T>
	inline bool Matrix4x4<T>::operator!=(const Matrix4x4<T>& aMatrix) const
	{
		for (size_t row = 0; row < myValues.size(); row++)
		{
			for (size_t column = 0; column < myValues[0].size(); column++)
			{
				if (myValues[row][column] == aMatrix.myValues[row][column])
				{
					return false;
				}
			}
		}
		return true;
	}
}