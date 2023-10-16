#pragma once
#include "Vector3.hpp"
#include "Matrix4x4.hpp"
#include "Math.hpp"
#include <array>

namespace Crimson
{
	template <typename T>
	class Matrix3x3
	{
	public:
		static const Matrix3x3<T> Null;

		// Creates the identity matrix.
		Matrix3x3<T>();
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix) = default;
		Matrix3x3<T>(Matrix3x3<T>&& aMatrix) = default;
		// Copies the top left 3x3 part of the Matrix4x4.
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);
		Matrix3x3<T>(std::array<std::array<T, 3>, 3> anArray);
		Matrix3x3<T>(const Vector3<T>& aFirstRow, const Vector3<T>& aSecondRow = Vector3<T>(T(), T(1), T()), const Vector3<T>& aThirdRow = Vector3<T>(T(), T(), T(1)));
		~Matrix3x3() = default;

		// Inserts the 3x3 part in the top left of the identity Matrix4x4
		operator Matrix4x4<T>() const;
		inline Matrix3x3<T>& operator=(const Matrix3x3<T>& aMatrix) = default;
		inline Matrix3x3<T>& operator=(Matrix3x3<T>&& aMatrix) = default;

		T& operator()(const unsigned int aRow, const unsigned int aColumn);
		const T& operator()(const unsigned int aRow, const unsigned int aColumn) const;

		static Matrix3x3<T> CreateRotationAroundX(const T& anAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(const T& anAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(const T& anAngleInRadians);

		static Matrix3x3<T> CreateScaleMatrix(const Vector3<T>& aScale);
		static Matrix3x3<T> CreateRotationMatrix(const Vector3<T>& aRotation);

		// Assumes aTransform is made up of nothing but rotations and translations.
		static Matrix3x3<T> GetFastInverse2D(const Matrix3x3<T>& aTransform);
		inline Matrix3x3<T> GetFastInverse2D() const;
		static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose);
		inline Matrix3x3<T> Transpose() const;

		inline Vector3<T> GetForward() const;
		inline Vector3<T> GetUp() const;
		inline Vector3<T> GetRight() const;

		inline void SetForward(const Vector3<T>& aVector);
		inline void SetUp(const Vector3<T>& aVector);
		inline void SetRight(const Vector3<T>& aVector);

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

	private:
		std::array<std::array<T, 3>, 3> myValues;
	};

	template<typename T> const Matrix3x3<T> Matrix3x3<T>::Null{};

	typedef Matrix3x3<float> Matrix3x3f;
	typedef Matrix3x3<int>  Matrix3x3i;

	template<typename T>
	Vector3<T>& operator*=(Vector3<T>& aRowVector, const Matrix3x3<T>& aMatrix) { return aRowVector = aMatrix * aRowVector; }
	template<typename T>
	Vector3<T> operator*(const Vector3<T>& aRowVector, const Matrix3x3<T>& aMatrix) { return aMatrix * aRowVector; }

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3() : myValues{ {{ T(1), T(), T() }, { T(), T(1), T() }, { T(), T(), T(1) }} }
	{
	}

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix) : myValues{ {{aMatrix(1,1), aMatrix(1,2), aMatrix(1,3) },
		{ aMatrix(2,1), aMatrix(2,2), aMatrix(2,3) }, { aMatrix(3,1), aMatrix(3,2), aMatrix(3,3) }} }
	{
	}

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3(std::array<std::array<T, 3>, 3> anArray): myValues(anArray)
	{
	}

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3(const Vector3<T>& aFirstRow, const Vector3<T>& aSecondRow, const Vector3<T>& aThirdRow) : myValues{ {
		{aFirstRow.x, aFirstRow.y, aFirstRow.z},
		{aSecondRow.x, aSecondRow.y, aSecondRow.z},
		{aThirdRow.x, aThirdRow.y, aThirdRow.z}} }
	{
	}

	template<typename T>
	inline Matrix3x3<T>::operator Matrix4x4<T>() const
	{
		return { {myValues[0][0],myValues[0][1],myValues[0][2],T()},
			{myValues[1][0],myValues[1][1],myValues[1][2],T()},
			{myValues[2][0],myValues[2][1],myValues[2][2],T()},
			{T(),T(),T(),T(1)} };
	}

	template<typename T>
	inline T& Matrix3x3<T>::operator()(const unsigned int aRow, const unsigned int aColumn)
	{
		assert(aRow != 0 && aColumn != 0);
		return myValues[aRow - 1][aColumn - 1];
	}

	template<typename T>
	inline const T& Matrix3x3<T>::operator()(const unsigned int aRow, const unsigned int aColumn) const
	{
		assert(aRow != 0 && aColumn != 0);
		return myValues[aRow - 1][aColumn - 1];
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
			{T(-sin(anAngleInRadians)), T(cos(anAngleInRadians)), T()}, 
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
	inline Matrix3x3<T> Matrix3x3<T>::Transpose(const Matrix3x3<T>& aMatrixToTranspose)
	{
		Matrix3x3<T> transpose = aMatrixToTranspose;
		Swap(transpose(1, 2), transpose(2, 1));
		Swap(transpose(1, 3), transpose(3, 1));
		Swap(transpose(2, 3), transpose(3, 2));
		return transpose;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::Transpose() const
	{
		return Transpose(*this);
	}

	template<typename T>
	inline Vector3<T> Matrix3x3<T>::GetForward() const
	{
		return Vector3<T>(myValues[0][2], myValues[1][2], myValues[2][2]).GetNormalized();
	}

	template<typename T>
	inline Vector3<T> Matrix3x3<T>::GetUp() const
	{
		return Vector3<T>(myValues[0][1], myValues[1][1], myValues[2][1]).GetNormalized();
	}

	template<typename T>
	inline Vector3<T> Matrix3x3<T>::GetRight() const
	{
		return Vector3<T>(myValues[0][0], myValues[1][0], myValues[2][0]).GetNormalized();
	}

	template<typename T>
	inline void Matrix3x3<T>::SetForward(const Vector3<T>& aVector)
	{
		myValues[0][2] = aVector.x;
		myValues[1][2] = aVector.y;
		myValues[2][2] = aVector.z;
	}

	template<typename T>
	inline void Matrix3x3<T>::SetUp(const Vector3<T>& aVector)
	{
		myValues[0][1] = aVector.x;
		myValues[1][1] = aVector.y;
		myValues[2][1] = aVector.z;
	}

	template<typename T>
	inline void Matrix3x3<T>::SetRight(const Vector3<T>& aVector)
	{
		myValues[0][0] = aVector.x;
		myValues[1][0] = aVector.y;
		myValues[2][0] = aVector.z;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::GetFastInverse2D(const Matrix3x3<T>& aTransform)
	{
		Matrix3x3<T> result{
			{aTransform(1,1),aTransform(2,1),aTransform(1,3)},
			{aTransform(1,2),aTransform(2,2),aTransform(2,3)},
			{T(),T(),T(1)} };
		Vector3<T> translate(-aTransform(3, 1), -aTransform(3, 2), aTransform(3, 3));
		translate = translate * result;
		result(3, 1) = translate.x;
		result(3, 2) = translate.y;
		return result;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::GetFastInverse2D() const
	{
		return GetFastInverse2D(*this);
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator+=(const Matrix3x3<T>& aMatrix)
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
	inline Matrix3x3<T> Matrix3x3<T>::operator+(const Matrix3x3<T>& aMatrix) const
	{
		Matrix3x3<T> result = *this;
		return result += aMatrix;
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator+=(const T& aScalar)
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
	inline Matrix3x3<T> Matrix3x3<T>::operator+(const T& aScalar) const
	{
		Matrix3x3<T> result = *this;
		return result += aScalar;
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator-=(const Matrix3x3<T>& aMatrix)
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
	inline Matrix3x3<T> Matrix3x3<T>::operator-(const Matrix3x3<T>& aMatrix) const
	{
		Matrix3x3<T> result = *this;
		return result -= aMatrix;
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator-=(const T& aScalar)
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
	inline Matrix3x3<T> Matrix3x3<T>::operator-(const T& aScalar) const
	{
		Matrix3x3<T> result = *this;
		return result -= aScalar;
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator*=(const Matrix3x3<T>& aMatrix)
	{
		Vector3<T> firstColumn(aMatrix.myValues[0][0], aMatrix.myValues[1][0], aMatrix.myValues[2][0]);
		Vector3<T> secondColumn(aMatrix.myValues[0][1], aMatrix.myValues[1][1], aMatrix.myValues[2][1]);
		Vector3<T> thirdColumn(aMatrix.myValues[0][2], aMatrix.myValues[1][2], aMatrix.myValues[2][2]);

		Vector3<T> firstRow(myValues[0][0], myValues[0][1], myValues[0][2]);
		Vector3<T> secondRow(myValues[1][0], myValues[1][1], myValues[1][2]);
		Vector3<T> thirdRow(myValues[2][0], myValues[2][1], myValues[2][2]);

		myValues[0][0] = firstRow.Dot(firstColumn);
		myValues[0][1] = firstRow.Dot(secondColumn);
		myValues[0][2] = firstRow.Dot(thirdColumn);

		myValues[1][0] = secondRow.Dot(firstColumn);
		myValues[1][1] = secondRow.Dot(secondColumn);
		myValues[1][2] = secondRow.Dot(thirdColumn);

		myValues[2][0] = thirdRow.Dot(firstColumn);
		myValues[2][1] = thirdRow.Dot(secondColumn);
		myValues[2][2] = thirdRow.Dot(thirdColumn);
		return *this;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::operator*(const Matrix3x3<T>& aMatrix) const
	{
		Matrix3x3<T> result = *this;
		return result *= aMatrix;
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator*=(const T& aScalar)
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
	inline Matrix3x3<T> Matrix3x3<T>::operator*(const T& aScalar) const
	{
		Matrix3x3<T> result = *this;
		return result *= aScalar;
	}

	template<typename T>
	inline Vector3<T> Matrix3x3<T>::operator*(const Vector3<T>& aRowVector) const
	{
		Vector3<T> result;
		result.x = (myValues[0][0] * aRowVector.x) + (myValues[1][0] * aRowVector.y) + (myValues[2][0] * aRowVector.z);
		result.y = (myValues[0][1] * aRowVector.x) + (myValues[1][1] * aRowVector.y) + (myValues[2][1] * aRowVector.z);
		result.z = (myValues[0][2] * aRowVector.x) + (myValues[1][2] * aRowVector.y) + (myValues[2][2] * aRowVector.z);
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
	inline Matrix3x3<T> Matrix3x3<T>::operator/(const T& aScalar) const
	{
		Matrix3x3<T> result = *this;
		return result /= aScalar;
	}

	template<typename T>
	inline bool Matrix3x3<T>::operator==(const Matrix3x3<T>& aMatrix) const
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
	inline bool Matrix3x3<T>::operator!=(const Matrix3x3<T>& aMatrix) const
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