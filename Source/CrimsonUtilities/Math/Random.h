#pragma once
#include <random>

namespace Crimson
{
	class Random
	{
	public:
		~Random() = default;

		// Generates a random number between 0 and anAmount, if anAmount is negative it is between anAmount and 0
		static int RandomNumber(int anAmount);
		static int RandomNumber(int aMinValue, int aMaxValue);

		// Generates a random number between 0 and anAmount, if anAmount is negative it is between anAmount and 0
		static float RandomNumber(float anAmount);
		static float RandomNumber(float aMinValue, float aMaxValue);

		// Generates a random number between 0 and anAmount, if anAmount is negative it is between anAmount and 0
		static double RandomNumber(double anAmount);
		static double RandomNumber(double aMinValue, double aMaxValue);

	private:
		static std::random_device mySeed;
		static std::mt19937 myEngine;
		
		Random() = default;
	};
}
