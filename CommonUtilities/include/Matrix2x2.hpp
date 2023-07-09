#pragma once
#include "Vector2.hpp"
#include "UtilityFunctions.hpp"
#include <array>

namespace CommonUtilities 
{
	template <typename T>
	class Matrix2x2
	{
	public:
		static const Matrix2x2<T> Null;

		// Creates the identity matrix.
		Matrix2x2<T>();
		Matrix2x2<T>(const Matrix2x2<T>& aMatrix) = default;
		Matrix2x2<T>(Matrix2x2<T>&& aMatrix) = default;
		Matrix2x2<T>(std::array<std::array<T, 2>, 2> anArray);
		Matrix2x2<T>(const Vector2<T>& aFirstRow, const Vector2<T>& aSecondRow = Vector2<T>(T(), T(1)));
		~Matrix2x2() = default;

		inline Matrix2x2<T>& operator=(const Matrix2x2<T>& aMatrix) = default;
		inline Matrix2x2<T>& operator=(Matrix2x2<T>&& aMatrix) = default;

		T& operator()(const unsigned int aRow, const unsigned int aColumn);
		const T& operator()(const unsigned int aRow, const unsigned int aColumn) const;

		static Matrix2x2<T> CreateRotationMatrix(const T& anAngleInRadians);
		static Matrix2x2<T> Transpose(const Matrix2x2<T>& aMatrixToTranspose);

		inline Matrix2x2<T>& operator+=(const Matrix2x2<T>& aMatrix);
		inline Matrix2x2<T> operator+(const Matrix2x2<T>& aMatrix) const;
		inline Matrix2x2<T>& operator+=(const T& aScalar);
		inline Matrix2x2<T> operator+(const T& aScalar) const;

		inline Matrix2x2<T>& operator-=(const Matrix2x2<T>& aMatrix);
		inline Matrix2x2<T> operator-(const Matrix2x2<T>& aMatrix) const;
		inline Matrix2x2<T>& operator-=(const T& aScalar);
		inline Matrix2x2<T> operator-(const T& aScalar) const;

		inline Matrix2x2<T>& operator*= (const Matrix2x2<T>& aMatrix);
		inline Matrix2x2<T> operator*(const Matrix2x2<T>& aMatrix) const;
		inline Matrix2x2<T>& operator*=(const T& aScalar);
		inline Matrix2x2<T> operator*(const T& aScalar) const;

		inline Vector2<T> operator*(const Vector2<T>& aRowVector) const;

		inline Matrix2x2<T>& operator/=(const T& aScalar);
		inline Matrix2x2<T> operator/(const T& aScalar) const;

		inline bool operator==(const Matrix2x2<T>& aMatrix) const;
		inline bool operator!=(const Matrix2x2<T>& aMatrix) const;

	private:
		std::array<std::array<T, 2>, 2> myValues;
	};

	template<typename T> const Matrix2x2<T> Matrix2x2<T>::Null{};

	typedef Matrix2x2<float> Matrix2x2f;
	typedef Matrix2x2<int>  Matrix2x2i;

	template<typename T>
	Vector2<T>& operator*=(Vector2<T>& aRowVector, const Matrix2x2<T>& aMatrix) { return aRowVector = aMatrix * aRowVector; }
	template<typename T>
	Vector2<T> operator*(const Vector2<T>& aRowVector, const Matrix2x2<T>& aMatrix) { return aMatrix * aRowVector; }

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename T>
	inline Matrix2x2<T>::Matrix2x2() : myValues{ {{ T(1), T() }, { T(), T(1) }} }
	{
	}

	template<typename T>
	inline Matrix2x2<T>::Matrix2x2(std::array<std::array<T, 2>, 2> anArray) : myValues(anArray)
	{
	}

	template<typename T>
	inline Matrix2x2<T>::Matrix2x2(const Vector2<T>& aFirstRow, const Vector2<T>& aSecondRow) : myValues{ {
		{aFirstRow.x, aFirstRow.y},
		{aSecondRow.x, aSecondRow.y}} }
	{
	}

	template<typename T>
	inline T& Matrix2x2<T>::operator()(const unsigned int aRow, const unsigned int aColumn)
	{
		assert(aRow != 0 && aColumn != 0);
		return myValues[aRow - 1][aColumn - 1];
	}

	template<typename T>
	inline const T& Matrix2x2<T>::operator()(const unsigned int aRow, const unsigned int aColumn) const
	{
		assert(aRow != 0 && aColumn != 0);
		return myValues[aRow - 1][aColumn - 1];
	}

	template<typename T>
	inline Matrix2x2<T> Matrix2x2<T>::CreateRotationMatrix(const T& anAngleInRadians)
	{
		return Matrix2x2<T>{ { T(cos(anAngleInRadians)), T(sin(anAngleInRadians)) },
				{ T(-sin(anAngleInRadians)), T(cos(anAngleInRadians)) } };
	}

	template<typename T>
	inline Matrix2x2<T> Matrix2x2<T>::Transpose(const Matrix2x2<T>& aMatrixToTranspose)
	{
		Matrix2x2<T> transpose = aMatrixToTranspose;
		Swap(transpose(1, 2), transpose(2, 1));
		return transpose;
	}

	template<typename T>
	inline Matrix2x2<T>& Matrix2x2<T>::operator+=(const Matrix2x2<T>& aMatrix)
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
	inline Matrix2x2<T> Matrix2x2<T>::operator+(const Matrix2x2<T>& aMatrix) const
	{
		Matrix2x2<T> result = *this;
		return result += aMatrix;
	}

	template<typename T>
	inline Matrix2x2<T>& Matrix2x2<T>::operator+=(const T& aScalar)
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
	inline Matrix2x2<T> Matrix2x2<T>::operator+(const T& aScalar) const
	{
		Matrix2x2<T> result = *this;
		return result += aScalar;
	}

	template<typename T>
	inline Matrix2x2<T>& Matrix2x2<T>::operator-=(const Matrix2x2<T>& aMatrix)
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
	inline Matrix2x2<T> Matrix2x2<T>::operator-(const Matrix2x2<T>& aMatrix) const
	{
		Matrix2x2<T> result = *this;
		return result -= aMatrix;
	}

	template<typename T>
	inline Matrix2x2<T>& Matrix2x2<T>::operator-=(const T& aScalar)
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
	inline Matrix2x2<T> Matrix2x2<T>::operator-(const T& aScalar) const
	{
		Matrix2x2<T> result = *this;
		return result -= aScalar;
	}

	template<typename T>
	inline Matrix2x2<T>& Matrix2x2<T>::operator*=(const Matrix2x2<T>& aMatrix)
	{
		Vector2<T> firstColumn(aMatrix.myValues[0][0], aMatrix.myValues[1][0]);
		Vector2<T> secondColumn(aMatrix.myValues[0][1], aMatrix.myValues[1][1]);

		Vector2<T> firstRow(myValues[0][0], myValues[0][1]);
		Vector2<T> secondRow(myValues[1][0], myValues[1][1]);

		myValues[0][0] = firstRow.Dot(firstColumn);
		myValues[0][1] = firstRow.Dot(secondColumn);

		myValues[1][0] = secondRow.Dot(firstColumn);
		myValues[1][1] = secondRow.Dot(secondColumn);
		return *this;
	}

	template<typename T>
	inline Matrix2x2<T> Matrix2x2<T>::operator*(const Matrix2x2<T>& aMatrix) const
	{
		Matrix2x2<T> result = *this;
		return result *= aMatrix;
	}

	template<typename T>
	inline Matrix2x2<T>& Matrix2x2<T>::operator*=(const T& aScalar)
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
	inline Matrix2x2<T> Matrix2x2<T>::operator*(const T& aScalar) const
	{
		Matrix2x2<T> result = *this;
		return result *= aScalar;
	}

	template<typename T>
	inline Vector2<T> Matrix2x2<T>::operator*(const Vector2<T>& aRowVector) const
	{
		Vector2<T> result;
		result.x = (myValues[0][0] * aRowVector.x) + (myValues[1][0] * aRowVector.y);
		result.y = (myValues[0][1] * aRowVector.x) + (myValues[1][1] * aRowVector.y);
		return result;
	}

	template<typename T>
	inline Matrix2x2<T>& Matrix2x2<T>::operator/=(const T& aScalar)
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
	inline Matrix2x2<T> Matrix2x2<T>::operator/(const T& aScalar) const
	{
		Matrix2x2<T> result = *this;
		return result /= aScalar;
	}

	template<typename T>
	inline bool Matrix2x2<T>::operator==(const Matrix2x2<T>& aMatrix) const
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
	inline bool Matrix2x2<T>::operator!=(const Matrix2x2<T>& aMatrix) const
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