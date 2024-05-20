#pragma once
#include "Json/JsonVector.hpp"

struct EmitterData
{
	float SpawnRate;
	float LifeTime;
	float GravityScale;
	float StartSpeed;
	float EndSpeed;
	Crimson::Vector3f StartSize;
	Crimson::Vector3f EndSize;
	Crimson::Vector4f StartColor;
	Crimson::Vector4f EndColor;

	EmitterData() : SpawnRate(0.f), LifeTime(0.f), GravityScale(1.f), StartSpeed(0.f), EndSpeed(0.f), StartSize(Crimson::Vector3f(1.f)), EndSize(Crimson::Vector3f(1.f))
	{}

	EmitterData(const Json::Value& aJson) : SpawnRate(aJson["SpawnRate"].asFloat()), LifeTime(aJson["LifeTime"].asFloat()), GravityScale(aJson["GravityScale"].asFloat()), StartSpeed(aJson["StartSpeed"].asFloat()),
		EndSpeed(aJson["EndSpeed"].asFloat()), StartSize(aJson["StartSize"]), EndSize(aJson["EndSize"]), StartColor(aJson["StartColor"]), EndColor(aJson["EndColor"])
	{}

	inline operator Json::Value() const
	{
		Json::Value result;
		result["SpawnRate"] = SpawnRate;
		result["LifeTime"] = LifeTime;
		result["GravityScale"] = GravityScale;
		result["StartSpeed"] = StartSpeed;
		result["EndSpeed"] = EndSpeed;
		result["StartSize"] = Json::Value(StartSize);
		result["EndSize"] = Json::Value(EndSize);
		result["StartColor"] = Json::Value(StartColor);
		result["EndColor"] = Json::Value(EndColor);
		return result;
	}
};