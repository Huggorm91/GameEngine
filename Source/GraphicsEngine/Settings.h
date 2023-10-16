#pragma once
namespace Json{ class Value; }

struct Settings
{
	std::string DefaultMissingTexture;
	std::string DefaultNormalTexture;
	std::string DefaultMaterialTexture;
	std::string DefaultFXTexture;
	std::string DefaultCubeMap;

	std::string DefaultMaterial;

	std::string LuminancePS{};
	std::string BlurPS{};
	std::string BloomPS{};
	std::string GammaPS{};
	std::string CopyPS{};

	std::string GBufferPS;
	std::string EnvironmentPS;
	std::string PointlightPS;
	std::string SpotlightPS;

	Crimson::Vector4f BackgroundColor;
	int ToneMap;

	Settings() = default;
	Settings(const Json::Value& aJson, const std::string& anAssetPath);
	operator Json::Value() const;
};