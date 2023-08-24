#pragma once

namespace CommonUtilities
{
	const double globalDegreeToRadianMultiplier = 0.01745329251994329576923690768489;
	const double globalRadianToDegreeMultiplier = 57.295779513082320876798154814105;

	template <typename T>
	float DegreeToRadian(T aDegree)
	{
		return static_cast<float>(aDegree * globalDegreeToRadianMultiplier);
	}	

	template <typename T>
	double DegreeToRadianPrecise(T aDegree)
	{
		return aDegree * globalDegreeToRadianMultiplier;
	}

	template <typename T>
	float RadianToDegree(T aRadian)
	{
		return static_cast<float>(aRadian * globalRadianToDegreeMultiplier);
	}

	template <typename T>
	double RadianToDegreePrecise(T aRadian)
	{
		return aRadian * globalRadianToDegreeMultiplier;
	}

	inline std::string WStringToString(const std::wstring& someString)
	{
		const int sLength = static_cast<int>(someString.length());
		const int len = WideCharToMultiByte(CP_ACP, 0, someString.c_str(), sLength, 0, 0, 0, 0);
		std::string result(len, L'\0');
		WideCharToMultiByte(CP_ACP, 0, someString.c_str(), sLength, &result[0], len, 0, 0);
		return result;
	}

	inline std::wstring StringToWString(const std::string& someString)
	{
		const int sLength = static_cast<int>(someString.length());
		const int len = MultiByteToWideChar(CP_ACP, 0, someString.c_str(), sLength, 0, 0);
		std::wstring result(len, L'\0');
		MultiByteToWideChar(CP_ACP, 0, someString.c_str(), sLength, &result[0], len);
		return result;
	}
}