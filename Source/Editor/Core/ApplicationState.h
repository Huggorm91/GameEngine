#pragma once
#include "CrimsonUtilities/Math/Vector2.hpp"
namespace Json{ class Value; }

struct ApplicationState
{
	bool startMaximized{};
	float shadowBias{};
	float ambientIntensity{};
	float cameraSpeed{};
	float cameraMouseSensitivity{};
	Crimson::Vector2i windowSize{};
	std::string windowTitle {};

	ApplicationState() = default;
	ApplicationState(const Json::Value& aJSon);
	operator Json::Value() const;
};