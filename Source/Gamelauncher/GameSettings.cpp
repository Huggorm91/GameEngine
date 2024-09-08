#include "GameLauncher.pch.h"
#include "GameSettings.h"
#include "CrimsonUtilities/Json/JsonVector.hpp"

GameSettings::GameSettings(const Json::Value& aJson) :
	isMaximized(aJson["StartMaximized"].asBool()),
	shadowBias(aJson["ShadowBias"].asFloat()),
	ambientIntensity(aJson["AmbientIntensity"].asFloat()),
	windowSize(aJson["WindowSize"]),
	windowTitle(aJson["WindowTitle"].asString())
{
}

GameSettings::operator Json::Value() const
{
	Json::Value result;
	result["StartMaximized"] = isMaximized;
	result["ShadowBias"] = shadowBias;
	result["AmbientIntensity"] = ambientIntensity;
	result["WindowSize"] = static_cast<Json::Value>(windowSize);
	result["WindowTitle"] = windowTitle;
	return result;
}