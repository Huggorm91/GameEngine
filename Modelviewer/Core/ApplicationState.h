#pragma once
#include "Json/JsonVector.hpp"

struct ApplicationState
{
	bool StartMaximized{};
	float ShadowBias{};
	float AmbientIntensity{};
	float CameraSpeed{};
	float CameraRotationSpeed{};
	float CameraMouseSensitivity{};
	Crimson::Vector2i WindowSize{};
	std::string WindowTitle {};

	ApplicationState() = default;
	ApplicationState(const Json::Value& aJSon);
	operator Json::Value() const;
};