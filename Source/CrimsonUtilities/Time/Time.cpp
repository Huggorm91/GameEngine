#include "Time.h"

std::chrono::high_resolution_clock::time_point Crimson::Time::myOldTime;
std::chrono::high_resolution_clock::time_point Crimson::Time::myCurrentTime;
std::chrono::duration<float> Crimson::Time::myDeltatime;
std::chrono::duration<double> Crimson::Time::myTotalTime;
float Crimson::Time::myTimeScale = 1.f;
float Crimson::Time::myScaledDeltaTime = 0.f;
double Crimson::Time::myScaledTotalTime = 0.f;

void Crimson::Time::Init()
{
    myOldTime = std::chrono::high_resolution_clock::now();
    myCurrentTime = myOldTime;
}

void Crimson::Time::Update()
{
    myCurrentTime = std::chrono::high_resolution_clock::now();

    myDeltatime = myCurrentTime - myOldTime;
    myTotalTime += myDeltatime;

    myScaledDeltaTime = myDeltatime.count() * myTimeScale;
    myScaledTotalTime += myScaledDeltaTime;

    myOldTime = myCurrentTime;
}

void Crimson::Time::SetTimeScale(float aScale)
{
    myTimeScale = aScale;
}

float Crimson::Time::GetTimeScale()
{
    return myTimeScale;
}

float Crimson::Time::GetDeltaTime()
{
    return myScaledDeltaTime;
}

double Crimson::Time::GetTotalTime()
{
    return myScaledTotalTime;
}

float Crimson::Time::GetUnscaledDeltaTime()
{
    return myDeltatime.count();
}

double Crimson::Time::GetUnscaledTotalTime()
{
    return myTotalTime.count();
}

std::chrono::high_resolution_clock::time_point Crimson::Time::StartTimer()
{
    return std::chrono::high_resolution_clock::now();
}

double Crimson::Time::StopTimer(const std::chrono::high_resolution_clock::time_point& aTimePoint)
{
    std::chrono::duration<double> result = std::chrono::high_resolution_clock::now() - aTimePoint;
    return result.count();
}

void Crimson::Time::ResetDeltaTime()
{
    myOldTime = std::chrono::high_resolution_clock::now();
    myDeltatime -= myDeltatime;
    myScaledDeltaTime = 0.f;
}

void Crimson::Time::ResetTotalTime()
{
    myTotalTime -= myTotalTime;
    myScaledTotalTime = 0.;
}
