#include "GraphicsEngine.pch.h"
#include "Settings.h"
#include "CrimsonUtilities/Json/JsonVector.hpp"
#include "AssetManager/AssetManager.h"

using namespace Crimson;

Settings::Settings(const Json::Value& aJson, const std::string& anAssetPath)
{
	auto& material = aJson["DefaultMaterial"];
	DefaultMaterial.Shininess = material["Shininess"].asFloat();
	DefaultMaterial.Metalness = material["Metalness"].asFloat();
	DefaultMaterial.NormalStrength = material["NormalStrength"].asFloat();
	DefaultMaterial.UVTiling = Crimson::Vector2f(material["UVTiling"]);
	DefaultMaterial.AlbedoColor = Crimson::Vector4f(material["AlbedoColor"]);
	DefaultMaterial.EmissionColor = Crimson::Vector4f(material["EmissionColor"]);
	DefaultMaterial.EmissionIntensity = material["EmissionIntensity"].asFloat();

	const std::string& textureExtension = ".dds";
	DefaultMaterialTexture = anAssetPath + AddExtensionIfMissing(aJson["MaterialTexture"].asString(), textureExtension);
	DefaultAlbedoTexture = anAssetPath + AddExtensionIfMissing(aJson["AlbedoTexture"].asString(), textureExtension);
	DefaultNormalTexture = anAssetPath + AddExtensionIfMissing(aJson["NormalTexture"].asString(), textureExtension);
	DefaultMissingTexture = anAssetPath + AddExtensionIfMissing(aJson["MissingTexture"].asString(), textureExtension);
	DefaultFXTexture = anAssetPath + AddExtensionIfMissing(aJson["FXTexture"].asString(), textureExtension);
	DefaultCubeMap = anAssetPath + AddExtensionIfMissing(aJson["CubeMap"].asString(), textureExtension);

	BackgroundColor = static_cast<Crimson::Vector4f>(aJson["BackgroundColor"]);
	ToneMap = aJson["ToneMap"].asInt();

	std::string path = Crimson::ToString(GetShaderPath(anAssetPath));

	const std::string& shaderExtension = ".cso";
	DefaultVSShader = AddExtensionIfMissing(path + aJson["DefaultVSShader"].asString(), shaderExtension);
	DefaultPSShader = AddExtensionIfMissing(path + aJson["DefaultPSShader"].asString(), shaderExtension);

	LuminancePS = AddExtensionIfMissing(path + aJson["LuminancePSShader"].asString(), shaderExtension);
	BlurPS = AddExtensionIfMissing(path + aJson["BlurPSShader"].asString(), shaderExtension);
	BloomPS = AddExtensionIfMissing(path + aJson["BloomPSShader"].asString(), shaderExtension);
	GammaPS = AddExtensionIfMissing(path + aJson["GammaPSShader"].asString(), shaderExtension);
	CopyPS = AddExtensionIfMissing(path + aJson["CopyPSShader"].asString(), shaderExtension);

	GBufferPS = AddExtensionIfMissing(path + aJson["GBufferPSShader"].asString(), shaderExtension);
	EnvironmentPS = AddExtensionIfMissing(path + aJson["EnvironmentPSShader"].asString(), shaderExtension);
	PointlightPS = AddExtensionIfMissing(path + aJson["PointlightPSShader"].asString(), shaderExtension);
	SpotlightPS = AddExtensionIfMissing(path + aJson["SpotlightPSShader"].asString(), shaderExtension);

	DefaultParticleVS = AddExtensionIfMissing(path + aJson["DefaultParticleVS"].asString(), shaderExtension);
	DefaultParticleGS = AddExtensionIfMissing(path + aJson["DefaultParticleGS"].asString(), shaderExtension);
	DefaultParticlePS = AddExtensionIfMissing(path + aJson["DefaultParticlePS"].asString(), shaderExtension);
}

Settings::operator Json::Value() const
{
	Json::Value json;
	json["MaterialTexture"] = DefaultMaterialTexture;
	json["NormalTexture"] = DefaultNormalTexture;
	json["MissingTexture"] = DefaultMissingTexture;
	json["FXTexture"] = DefaultFXTexture;
	json["CubeMap"] = DefaultCubeMap;

	json["DefaultMaterial"] = Json::objectValue;
	auto& material = json["DefaultMaterial"];
	material["Shininess"] = DefaultMaterial.Shininess;
	material["Metalness"] = DefaultMaterial.Metalness;
	material["NormalStrength"] = DefaultMaterial.NormalStrength;
	material["UVTiling"] = static_cast<Json::Value>(DefaultMaterial.UVTiling);
	material["AlbedoColor"] = DefaultMaterial.AlbedoColor.ToJsonColor();
	material["EmissionColor"] = DefaultMaterial.EmissionColor.ToJsonColor();
	material["EmissionIntensity"] = DefaultMaterial.EmissionIntensity;

	json["BackgroundColor"] = BackgroundColor.ToJsonColor();
	json["ToneMap"] = ToneMap;

	const std::string& comment = "// Only use 'ShaderName.cso' and not full path";
	size_t lastSlash = LuminancePS.find_last_of('/') + 1;
	json["LuminancePSShader"] = LuminancePS.substr(lastSlash);
	json["LuminancePSShader"].setComment(comment, Json::commentAfterOnSameLine);

	lastSlash = BlurPS.find_last_of('/') + 1;
	json["BlurPSShader"] = BlurPS.substr(lastSlash);
	json["BlurPSShader"].setComment(comment, Json::commentAfterOnSameLine);

	lastSlash = BloomPS.find_last_of('/') + 1;
	json["BloomPSShader"] = BloomPS.substr(lastSlash);
	json["BloomPSShader"].setComment(comment, Json::commentAfterOnSameLine);

	lastSlash = GammaPS.find_last_of('/') + 1;
	json["GammaPSShader"] = GammaPS.substr(lastSlash);
	json["GammaPSShader"].setComment(comment, Json::commentAfterOnSameLine);

	lastSlash = CopyPS.find_last_of('/') + 1;
	json["CopyPSShader"] = CopyPS.substr(lastSlash);
	json["CopyPSShader"].setComment(comment, Json::commentAfterOnSameLine);

	lastSlash = GBufferPS.find_last_of('/') + 1;
	json["GBufferPSShader"] = GBufferPS.substr(lastSlash);
	json["GBufferPSShader"].setComment(comment, Json::commentAfterOnSameLine);

	lastSlash = EnvironmentPS.find_last_of('/') + 1;
	json["EnvironmentPSShader"] = EnvironmentPS.substr(lastSlash);
	json["EnvironmentPSShader"].setComment(comment, Json::commentAfterOnSameLine);

	lastSlash = PointlightPS.find_last_of('/') + 1;
	json["PointlightPSShader"] = PointlightPS.substr(lastSlash);
	json["PointlightPSShader"].setComment(comment, Json::commentAfterOnSameLine);

	lastSlash = SpotlightPS.find_last_of('/') + 1;
	json["SpotlightPSShader"] = SpotlightPS.substr(lastSlash);
	json["SpotlightPSShader"].setComment(comment, Json::commentAfterOnSameLine);

	lastSlash = DefaultParticleVS.find_last_of('/') + 1;
	json["DefaultParticleVS"] = DefaultParticleVS.substr(lastSlash);
	json["DefaultParticleVS"].setComment(comment, Json::commentAfterOnSameLine);

	lastSlash = DefaultParticleGS.find_last_of('/') + 1;
	json["DefaultParticleGS"] = DefaultParticleGS.substr(lastSlash);
	json["DefaultParticleGS"].setComment(comment, Json::commentAfterOnSameLine);

	lastSlash = DefaultParticlePS.find_last_of('/') + 1;
	json["DefaultParticlePS"] = DefaultParticlePS.substr(lastSlash);
	json["DefaultParticlePS"].setComment(comment, Json::commentAfterOnSameLine);

	return json;
}