#pragma once
#include <Vector4.hpp>

namespace Json{ class Value; }

struct Settings
{
	std::string defaultMissingTexture;
	std::string defaultNormalTexture;
	std::string defaultMaterialTexture;
	std::string defaultFXTexture;
	std::string defaultCubeMap;

	std::string defaultMaterial;

	std::string defaultGBufferPSShader;
	std::string defaultEnvironmentPSShader;
	std::string defaultPointlightPSShader;
	std::string defaultSpotlightPSShader;

	CommonUtilities::Vector4f backgroundColor;

	Settings() = default;
	Settings(const Json::Value& aJson);
	operator Json::Value() const;
};