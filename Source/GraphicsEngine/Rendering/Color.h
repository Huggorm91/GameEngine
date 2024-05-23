#pragma once
#include <unordered_map>
#include "CrimsonUtilities/Math/Vector4.hpp"

class ColorManager
{
public:
	ColorManager() = default;
	~ColorManager() = default;

	static void Init(const Json::Value& someData);
	static Json::Value ToJson();

	static void AddColor(const std::string& aName, const Crimson::Vector4f& aColor);
	static void RemoveColor(const std::string& aName);

	static Crimson::Vector4f GetColor(const std::string& aName);
	static Crimson::Vector4f GetColor(const Crimson::Vector3f& aHexValue);

	static std::vector<std::string> GetNames();
	static bool CreateImGuiComponents(Crimson::Vector4f& outColor, std::string& outSelection, const std::string& aComboName = " ");
private:
	static std::unordered_map<std::string, Crimson::Vector4f> myColors;
};