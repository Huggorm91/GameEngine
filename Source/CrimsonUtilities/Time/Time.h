#pragma once
#include <chrono>

namespace Crimson
{
	class Time
	{
	public:
		Time(const Time&) = delete;
		Time& operator=(const Time&) = delete;
		~Time() = default;

		static void Init();
		static void Update();

		static void SetTimeScale(float aScale);
		static float GetTimeScale();

		static float GetDeltaTime();
		static double GetTotalTime();

		static float GetUnscaledDeltaTime();
		static double GetUnscaledTotalTime();

		static std::chrono::high_resolution_clock::time_point StartTimer();
		static double StopTimer(const std::chrono::high_resolution_clock::time_point& aTimePoint);

		static void ResetDeltaTime();
		static void ResetTotalTime();

	private:
		static std::chrono::high_resolution_clock::time_point myOldTime;
		static std::chrono::high_resolution_clock::time_point myCurrentTime;
		static std::chrono::duration<double> myTotalTime;
		static std::chrono::duration<float> myDeltatime;
		static double myScaledTotalTime;
		static float myScaledDeltaTime;
		static float myTimeScale;

		Time() = default;
	};
}