#include "Random.h"
#include <assert.h>

namespace Crimson
{
	std::random_device Random::mySeed;
	std::mt19937 Random::myEngine(mySeed());

	int Random::RandomNumber(int anAmount)
	{
		if (anAmount < 0)
		{
			std::uniform_int_distribution<int> rndNumber(anAmount, 0);
			return rndNumber(myEngine);
		}
		else
		{
			std::uniform_int_distribution<int> rndNumber(0, anAmount);
			return rndNumber(myEngine);
		}
	}

	int Random::RandomNumber(int aMinValue, int aMaxValue)
	{
		assert(aMinValue <= aMaxValue);
		if (aMinValue > aMaxValue)
		{
			return 0;
		}

		std::uniform_int_distribution<int> rndNumber(aMinValue, aMaxValue);
		return rndNumber(myEngine);
	}

	float Random::RandomNumber(float anAmount)
	{
		if (anAmount < .0f)
		{
			std::uniform_real_distribution<float> rndNumber(anAmount, 0);
			return rndNumber(myEngine);
		}
		else
		{
			std::uniform_real_distribution<float> rndNumber(0, anAmount);
			return rndNumber(myEngine);
		}
	}

	float Random::RandomNumber(float aMinValue, float aMaxValue)
	{
		assert(aMinValue <= aMaxValue);
		if (aMinValue > aMaxValue)
		{
			return .0f;
		}

		std::uniform_real_distribution<float> rndNumber(aMinValue, aMaxValue);
		return rndNumber(myEngine);
	}

	double Random::RandomNumber(double anAmount)
	{
		if (anAmount < .0)
		{
			std::uniform_real_distribution<double> rndNumber(anAmount, 0);
			return rndNumber(myEngine);
		}
		else
		{
			std::uniform_real_distribution<double> rndNumber(0, anAmount);
			return rndNumber(myEngine);
		}
	}

	double Random::RandomNumber(double aMinValue, double aMaxValue)
	{
		assert(aMinValue <= aMaxValue);
		if (aMinValue > aMaxValue)
		{
			return .0;
		}

		std::uniform_real_distribution<double> rndNumber(aMinValue, aMaxValue);
		return rndNumber(myEngine);
	}
}