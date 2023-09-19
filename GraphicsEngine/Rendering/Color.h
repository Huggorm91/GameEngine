#pragma once
#include <JsonVector.hpp>
#include <unordered_map>

class ColorManager
{
public:
	ColorManager() = default;
	~ColorManager() = default;

	static void Init(const Json::Value& someData);
	static Json::Value ToJson();

	static void AddColor(const std::string& aName, const CommonUtilities::Vector4f& aColor);
	static void RemoveColor(const std::string& aName);

	static CommonUtilities::Vector4f GetColor(const std::string& aName);
	static CommonUtilities::Vector4f GetColor(const CommonUtilities::Vector3f& aHexValue);

	static std::vector<std::string> GetNames();
	static bool CreateImGuiComponents(CommonUtilities::Vector4f& outColor, std::string& outSelection, const std::string& aComboName = " ");
private:
	static std::unordered_map<std::string, CommonUtilities::Vector4f> myColors;
};