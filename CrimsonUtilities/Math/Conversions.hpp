#pragma once

namespace Crimson
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
}