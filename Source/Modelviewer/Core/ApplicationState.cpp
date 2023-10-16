#include "ModelViewer.pch.h"
#include "ApplicationState.h"
#include "Json/JsonVector.hpp"

ApplicationState::ApplicationState(const Json::Value& aJson) : StartMaximized(aJson["StartMaximized"].asBool()), CameraSpeed(aJson["CameraSpeed"].asFloat()), CameraRotationSpeed(aJson["CameraRotationSpeed"].asFloat()), 
CameraMouseSensitivity(aJson["CameraMouseSensitivity"].asFloat()), WindowSize(aJson["WindowSize"]), WindowTitle(aJson["WindowTitle"].asString()), AmbientIntensity(aJson["AmbientIntensity"].asFloat()), ShadowBias(aJson["ShadowBias"].asFloat())
{
}

ApplicationState::operator Json::Value() const
{
	Json::Value result;
	result["StartMaximized"] = StartMaximized;
	result["CameraSpeed"] = CameraSpeed;
	result["CameraRotationSpeed"] = CameraRotationSpeed;
	result["CameraMouseSensitivity"] = CameraMouseSensitivity;
	result["WindowSize"] = static_cast<Json::Value>(WindowSize);
	result["WindowTitle"] = WindowTitle;
	result["AmbientIntensity"] = AmbientIntensity;
	result["ShadowBias"] = ShadowBias;
	return result;
}