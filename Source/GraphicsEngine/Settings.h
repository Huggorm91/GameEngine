#pragma once
#include "String/StringFunctions.h"
namespace Json{
	class Value;
}

struct Settings
{
	struct MaterialData
	{
		Crimson::Vector4f AlbedoColor;
		Crimson::Vector4f EmissionColor;
		float EmissionIntensity{};
		Crimson::Vector2f UVTiling;
		float NormalStrength{};
		float Shininess{};
		float Metalness{};
	}DefaultMaterial;

	std::string DefaultMissingTexture;
	std::string DefaultAlbedoTexture;
	std::string DefaultNormalTexture;
	std::string DefaultMaterialTexture;
	std::string DefaultFXTexture;
	std::string DefaultCubeMap;

	std::string DefaultVSShader;
	std::string DefaultPSShader;

	std::string LuminancePS;
	std::string BlurPS;
	std::string BloomPS;
	std::string GammaPS;
	std::string CopyPS;

	std::string GBufferPS;
	std::string EnvironmentPS;
	std::string PointlightPS;
	std::string SpotlightPS;

	Crimson::Vector4f BackgroundColor;
	int ToneMap{};

	Settings() = default;
	Settings(const Json::Value& aJson, const std::string& anAssetPath);
	operator Json::Value() const;

	static std::wstring GetShaderPath(const std::string& anAssetPath)
	{
#ifdef _DEBUG
		return Crimson::ToWString(anAssetPath) + L"Debug\\";
#elif _RELEASE
		return Crimson::ToWString(anAssetPath) + L"Release\\";
#elif _RETAIL
		return Crimson::ToWString(anAssetPath) + L"Retail\\";
#endif // _DEBUG 
	}
};