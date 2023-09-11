#include "GraphicsEngine.pch.h"
#include "Settings.h"
#include "External/jsonCpp/json.h"

Settings::Settings(const Json::Value& aJson)
{
	defaultMaterialTexture = aJson["MaterialTexture"].asString();
	defaultNormalTexture = aJson["NormalTexture"].asString();
	defaultMissingTexture = aJson["MissingTexture"].asString();
	defaultFXTexture = aJson["FXTexture"].asString();
	defaultCubeMap = aJson["CubeMap"].asString();

	defaultMaterial = aJson["DefaultMaterial"].asString();

	backgroundColor.x = aJson["BackgroundColor"]["R"].asFloat();
	backgroundColor.y = aJson["BackgroundColor"]["G"].asFloat();
	backgroundColor.z = aJson["BackgroundColor"]["B"].asFloat();
	backgroundColor.w = aJson["BackgroundColor"]["A"].asFloat();

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

	defaultGBufferPSShader = path + aJson["GBufferPSShader"].asString();
	defaultEnvironmentPSShader = path + aJson["EnvironmentPSShader"].asString();
	defaultPointlightPSShader = path + aJson["PointlightPSShader"].asString();
	defaultSpotlightPSShader = path + aJson["SpotlightPSShader"].asString();
}

Settings::operator Json::Value() const
{
	Json::Value json;
	json["MaterialTexture"] = defaultMaterialTexture;
	json["NormalTexture"] = defaultNormalTexture;
	json["MissingTexture"] = defaultMissingTexture;
	json["FXTexture"] = defaultFXTexture;
	json["CubeMap"] = defaultCubeMap;

	json["DefaultMaterial"] = defaultMaterial;

	json["BackgroundColor"]["R"] = backgroundColor.x;
	json["BackgroundColor"]["G"] = backgroundColor.y;
	json["BackgroundColor"]["B"] = backgroundColor.z;
	json["BackgroundColor"]["A"] = backgroundColor.w;

	const std::string comment = "// Only use 'ShaderName.cso' and not full path";
	size_t lastSlash = defaultGBufferPSShader.find_last_of('/') + 1;
	json["GBufferPSShader"] = defaultGBufferPSShader.substr(lastSlash);
	json["GBufferPSShader"].setComment(comment, Json::commentAfterOnSameLine);

	lastSlash = defaultEnvironmentPSShader.find_last_of('/') + 1;
	json["EnvironmentPSShader"] = defaultEnvironmentPSShader.substr(lastSlash);
	json["EnvironmentPSShader"].setComment(comment, Json::commentAfterOnSameLine);

	lastSlash = defaultPointlightPSShader.find_last_of('/') + 1;
	json["PointlightPSShader"] = defaultPointlightPSShader.substr(lastSlash);
	json["PointlightPSShader"].setComment(comment, Json::commentAfterOnSameLine);

	lastSlash = defaultSpotlightPSShader.find_last_of('/') + 1;
	json["SpotlightPSShader"] = defaultSpotlightPSShader.substr(lastSlash);
	json["SpotlightPSShader"].setComment(comment, Json::commentAfterOnSameLine);

	return json;
}