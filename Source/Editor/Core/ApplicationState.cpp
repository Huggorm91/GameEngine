#include "Editor.pch.h"
#include "ApplicationState.h"
#include "CrimsonUtilities/Json/JsonVector.hpp"

ApplicationState::ApplicationState(const Json::Value& aJson) : startMaximized(aJson["StartMaximized"].asBool()), cameraSpeed(aJson["CameraSpeed"].asFloat()), cameraMouseSensitivity(aJson["CameraMouseSensitivity"].asFloat()), 
windowSize(aJson["WindowSize"]), windowTitle(aJson["WindowTitle"].asString()), ambientIntensity(aJson["AmbientIntensity"].asFloat()), shadowBias(aJson["ShadowBias"].asFloat())
{
}

ApplicationState::operator Json::Value() const
{
	Json::Value result;
	result["StartMaximized"] = startMaximized;
	result["CameraSpeed"] = cameraSpeed;
	result["CameraMouseSensitivity"] = cameraMouseSensitivity;
	result["WindowSize"] = static_cast<Json::Value>(windowSize);
	result["WindowTitle"] = windowTitle;
	result["AmbientIntensity"] = ambientIntensity;
	result["ShadowBias"] = shadowBias;
	return result;
}