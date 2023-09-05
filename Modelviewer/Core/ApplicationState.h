#pragma once
#include <JsonVector.hpp>

struct ApplicationState
{
	bool StartMaximized{};
	float AmbientIntensity{};
	float CameraSpeed{};
	float CameraRotationSpeed{};
	float CameraMouseSensitivity{};
	CommonUtilities::Vector2i WindowSize{};
	std::string WindowTitle {};

	ApplicationState() = default;
	ApplicationState(const Json::Value& aJSon);
	operator Json::Value() const;
};