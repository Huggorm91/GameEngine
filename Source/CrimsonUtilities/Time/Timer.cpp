#include "Timer.h"

std::chrono::high_resolution_clock::time_point Crimson::Timer::myOldTime;
std::chrono::high_resolution_clock::time_point Crimson::Timer::myCurrentTime;
std::chrono::duration<float> Crimson::Timer::myDeltatime;
std::chrono::duration<double> Crimson::Timer::myTotalTime;

Crimson::Timer::Timer()
{    
}

void Crimson::Timer::Init()
{
    myOldTime = std::chrono::high_resolution_clock::now();
    myCurrentTime = myOldTime;
}

void Crimson::Timer::Update()
{
    myCurrentTime = std::chrono::high_resolution_clock::now();
    myDeltatime = myCurrentTime - myOldTime;
    myTotalTime += myDeltatime;
    myOldTime = myCurrentTime;
}

float Crimson::Timer::GetDeltaTime()
{
    return myDeltatime.count();
}

double Crimson::Timer::GetTotalTime()
{
    return myTotalTime.count();
}

std::chrono::high_resolution_clock::time_point Crimson::Timer::StartStopwatch()
{
    return std::chrono::high_resolution_clock::now();
}

double Crimson::Timer::StopStopwatch(const std::chrono::high_resolution_clock::time_point& aTimePoint)
{
    std::chrono::duration<double> result = std::chrono::high_resolution_clock::now() - aTimePoint;
    return result.count();
}

void Crimson::Timer::ResetDeltaTime()
{
    myOldTime = std::chrono::high_resolution_clock::now();
}