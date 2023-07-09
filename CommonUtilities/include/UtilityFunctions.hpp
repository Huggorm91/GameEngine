#pragma once
#include <assert.h>
#include <vector>
#include <sstream>
#include "Conversions.hpp" // Adds conversion with a single include
constexpr auto Pi = 3.14159;

namespace CommonUtilities
{
	template <typename T>
	const T& Max(const T& aFirst, const T& aSecond)
	{
		if (aFirst < aSecond)
		{
			return aSecond;
		}
		else
		{
			return aFirst;
		}
	}

	template <typename T>
	const T& Min(const T& aFirst, const T& aSecond)
	{
		if (aFirst < aSecond)
		{
			return aFirst;
		}
		else
		{
			return aSecond;
		}
	}

	template <typename T>
	T Abs(const T& aValue)
	{
		if (aValue < T(0))
		{
			return -aValue;
		}
		else
		{
			return aValue;
		}
	}

	template <typename T>
	const T& Clamp(const T& aValue, const T& aMin, const T& aMax)
	{
		assert(aMin <= aMax);
		if (aValue < aMin)
		{
			return aMin;
		}
		else if (aMax < aValue)
		{
			return aMax;
		}
		else
		{
			return aValue;
		}
	}

	template <typename T>
	T Lerp(const T& aFirst, const T& aSecond, const float aPercentage)
	{
		return static_cast<T>(aFirst + (aPercentage * (aSecond - aFirst)));
	}

	template <typename T>
	void Swap(T& anOutFirst, T& anOutSecond)
	{
		T temp = anOutFirst;
		anOutFirst = anOutSecond;
		anOutSecond = temp;
	}

	// The Classic Quake Algorithm
	template <typename T>
	T GetFastInverseSquareRoot(const T& aNumber)
	{
		long i;
		T x2, y;
		const float threehalfs = 1.5F;

		x2 = aNumber * 0.5F;
		y = aNumber;
		i = *(long*)&y;
		i = 0x5f3759df - (i >> 1);
		y = *(float*)&i;
		y = y * (threehalfs - (x2 * y * y));

		return y;
	}

	inline std::vector<std::string> SplitString(const std::string& anInput, const char aSplitter)
	{
		std::istringstream iss(anInput);
		std::string item;

		std::vector<std::string> output;

		while (std::getline(iss, item, aSplitter))
		{
			output.push_back(item);
		}
		return output;
	}

	// Does comparison using only operator <
	template <typename T>
	bool IsEqual(const T& aFirst, const T& aSecond)
	{
		return !(aFirst < aSecond) && !(aSecond < aFirst);
	}
}