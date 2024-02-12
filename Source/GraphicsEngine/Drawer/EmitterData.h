#pragma once
#include "Json/JsonVector.hpp"

struct EmitterData
{
	float SpawnRate;
	float LifeTime;
	float GravityScale;
	Crimson::Vector3f StartVelocity;
	Crimson::Vector3f EndVelocity;
	Crimson::Vector3f StartSize;
	Crimson::Vector3f EndSize;
	Crimson::Vector4f StartColor;
	Crimson::Vector4f EndColor;

	EmitterData() : SpawnRate(0.f), LifeTime(0.f), GravityScale(0.f)
	{}

	EmitterData(const Json::Value& aJson) : SpawnRate(aJson["SpawnRate"].asFloat()), LifeTime(aJson["LifeTime"].asFloat()), GravityScale(aJson["GravityScale"].asFloat()), StartVelocity(aJson["StartVelocity"]), 
		EndVelocity(aJson["EndVelocity"]), StartSize(aJson["StartSize"]), EndSize(aJson["EndSize"]), StartColor(aJson["StartColor"]), EndColor(aJson["EndColor"])
	{}

	inline operator Json::Value() const
	{
		Json::Value result;
		result["SpawnRate"] = SpawnRate;
		result["LifeTime"] = LifeTime;
		result["GravityScale"] = GravityScale;
		result["StartVelocity"] = Json::Value(StartVelocity);
		result["EndVelocity"] = Json::Value(EndVelocity);
		result["StartSize"] = Json::Value(StartSize);
		result["EndSize"] = Json::Value(EndSize);
		result["StartColor"] = Json::Value(StartColor);
		result["EndColor"] = Json::Value(EndColor);
		return result;
	}
};