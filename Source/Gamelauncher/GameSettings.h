#pragma once
#include "CrimsonUtilities/Math/Vector2.hpp"
namespace Json { class Value; }

struct GameSettings
{
	bool isMaximized{};
	float shadowBias{};
	float ambientIntensity{};
	Crimson::Vector2i windowSize{};
	std::string windowTitle{};

	GameSettings() = default;
	GameSettings(const Json::Value& aJson);
	operator Json::Value() const;
};

