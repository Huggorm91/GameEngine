#include "GraphicsEngine.pch.h"
#include "Settings.h"
#include "Json/JsonVector.hpp"
#include "AssetManager/AssetManager.h"
#include "File/DirectoryFunctions.h"

using namespace Crimson;

Settings::Settings(const Json::Value& aJson)
{
	const std::string& textureExtension = ".dds";
	DefaultMaterialTexture = AddExtensionIfMissing(aJson["MaterialTexture"].asString(), textureExtension);	
	DefaultNormalTexture = AddExtensionIfMissing(aJson["NormalTexture"].asString(), textureExtension);
	DefaultMissingTexture = AddExtensionIfMissing(aJson["MissingTexture"].asString(), textureExtension);
	DefaultFXTexture = AddExtensionIfMissing(aJson["FXTexture"].asString(), textureExtension);
	DefaultCubeMap = AddExtensionIfMissing(aJson["CubeMap"].asString(), textureExtension);

	DefaultMaterial = AddExtensionIfMissing(aJson["DefaultMaterial"].asString(), ".mat");

	BackgroundColor = static_cast<Crimson::Vector4f>(aJson["BackgroundColor"]);
	ToneMap = aJson["ToneMap"].asInt();

	std::string path = AssetManager::GetShaderPath();

	const std::string& shaderExtension = ".cso";	
	LuminancePS = AddExtensionIfMissing(path + aJson["LuminancePSShader"].asString(), shaderExtension);
	BlurPS = AddExtensionIfMissing(path + aJson["BlurPSShader"].asString(), shaderExtension);
	BloomPS = AddExtensionIfMissing(path + aJson["BloomPSShader"].asString(), shaderExtension);
	GammaPS = AddExtensionIfMissing(path + aJson["GammaPSShader"].asString(), shaderExtension);
	CopyPS = AddExtensionIfMissing(path + aJson["CopyPSShader"].asString(), shaderExtension);

	GBufferPS = AddExtensionIfMissing(path + aJson["GBufferPSShader"].asString(), shaderExtension);
	EnvironmentPS = AddExtensionIfMissing(path + aJson["EnvironmentPSShader"].asString(), shaderExtension);
	PointlightPS = AddExtensionIfMissing(path + aJson["PointlightPSShader"].asString(), shaderExtension);
	SpotlightPS = AddExtensionIfMissing(path + aJson["SpotlightPSShader"].asString(), shaderExtension);
}

Settings::operator Json::Value() const
{
	Json::Value json;
	json["MaterialTexture"] = DefaultMaterialTexture;
	json["NormalTexture"] = DefaultNormalTexture;
	json["MissingTexture"] = DefaultMissingTexture;
	json["FXTexture"] = DefaultFXTexture;
	json["CubeMap"] = DefaultCubeMap;

	json["DefaultMaterial"] = DefaultMaterial;

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

	return json;
}