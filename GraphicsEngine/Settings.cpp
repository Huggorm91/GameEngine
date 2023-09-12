#include "GraphicsEngine.pch.h"
#include "Settings.h"
#include "External/jsonCpp/json.h"

Settings::Settings(const Json::Value& aJson)
{
	DefaultMaterialTexture = aJson["MaterialTexture"].asString();
	DefaultNormalTexture = aJson["NormalTexture"].asString();
	DefaultMissingTexture = aJson["MissingTexture"].asString();
	DefaultFXTexture = aJson["FXTexture"].asString();
	DefaultCubeMap = aJson["CubeMap"].asString();

	DefaultMaterial = aJson["DefaultMaterial"].asString();

	BackgroundColor.x = aJson["BackgroundColor"]["R"].asFloat();
	BackgroundColor.y = aJson["BackgroundColor"]["G"].asFloat();
	BackgroundColor.z = aJson["BackgroundColor"]["B"].asFloat();
	BackgroundColor.w = aJson["BackgroundColor"]["A"].asFloat();

	std::string path;
#ifdef _DEBUG
	path = "Content/Shaders/Debug/";
#elif _RELEASE
	path = "Content/Shaders/Release/";
#elif _RETAIL
	path = "Content/Shaders/Retail/";
#else
	GELogger.Err("Settings: Invalid buildsettings in Settings(Json)!");
#endif // _DEBUG

	LuminancePS = path + aJson["LuminancePSShader"].asString();
	BlurPS = path + aJson["BlurPSShader"].asString();
	BloomPS = path + aJson["BloomPSShader"].asString();
	GammaPS = path + aJson["GammaPSShader"].asString();
	CopyPS = path + aJson["CopyPSShader"].asString();

	GBufferPS = path + aJson["GBufferPSShader"].asString();
	EnvironmentPS = path + aJson["EnvironmentPSShader"].asString();
	PointlightPS = path + aJson["PointlightPSShader"].asString();
	SpotlightPS = path + aJson["SpotlightPSShader"].asString();
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

	json["BackgroundColor"]["R"] = BackgroundColor.x;
	json["BackgroundColor"]["G"] = BackgroundColor.y;
	json["BackgroundColor"]["B"] = BackgroundColor.z;
	json["BackgroundColor"]["A"] = BackgroundColor.w;

	const std::string comment = "// Only use 'ShaderName.cso' and not full path";
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