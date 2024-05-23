#pragma once
#include "CrimsonUtilities/Math/Vector2.hpp"
namespace Json{ class Value; }

struct ApplicationState
{
	bool StartMaximized{};
	float ShadowBias{};
	float AmbientIntensity{};
	float CameraSpeed{};
	float CameraMouseSensitivity{};
	Crimson::Vector2i WindowSize{};
	std::string WindowTitle {};

	ApplicationState() = default;
	ApplicationState(const Json::Value& aJSon);
	operator Json::Value() const;
};