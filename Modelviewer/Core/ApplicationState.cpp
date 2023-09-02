#include "ApplicationState.h"

ApplicationState::ApplicationState(const Json::Value& aJSon) : StartMaximized(aJSon["StartMaximized"].asBool()), CameraSpeed(aJSon["CameraSpeed"].asFloat()), CameraRotationSpeed(aJSon["CameraRotationSpeed"].asFloat()), 
CameraMouseSensitivity(aJSon["CameraMouseSensitivity"].asFloat()), WindowSize(aJSon["WindowSize"]), WindowTitle(aJSon["WindowTitle"].asString())
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
	return result;
}