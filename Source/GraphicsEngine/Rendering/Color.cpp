#include "GraphicsEngine.pch.h"
#include "Color.h"
#include "Json/JsonVector.hpp"
#include "ImGui/imgui.h"
#include "ImGui/misc/cpp/imgui_stdlib.h"

std::unordered_map<std::string, Crimson::Vector4f> ColorManager::myColors{};

Crimson::Vector4f GetColor(const Crimson::Vector3f& aHexValue)
{
    return Crimson::Vector4f(aHexValue/255.f, 1.f);
}

void ColorManager::Init(const Json::Value& someData)
{
	for (auto& value : someData)
	{
		myColors.emplace(value["Name"].asString(), value["Color"]);
	}
}

Json::Value ColorManager::ToJson()
{
	Json::Value result = Json::arrayValue;
	int i = 0;
	for (auto& [name, color] : myColors)
	{
		result[i]["Name"] = name;
		result[i]["Color"] = color.ToJsonColor();
		i++;
	}
	return result;
}

void ColorManager::AddColor(const std::string& aName, const Crimson::Vector4f& aColor)
{
	myColors.emplace(aName, aColor);
}

void ColorManager::RemoveColor(const std::string& aName)
{
	if (auto iter = myColors.find(aName); iter != myColors.end())
	{
		myColors.erase(iter);
	}
}

Crimson::Vector4f ColorManager::GetColor(const std::string& aName)
{
	if (auto iter = myColors.find(aName); iter != myColors.end())
	{
		return iter->second;
	}
	return Crimson::Vector4f();
}

Crimson::Vector4f ColorManager::GetColor(const Crimson::Vector3f& aHexValue)
{
	return Crimson::Vector4f(aHexValue * 0.0039215686274509803921568627451f, 1.f); // aHexValue / 255
}

std::vector<std::string> ColorManager::GetNames()
{
	std::vector<std::string> result;
	for (auto& [name, color] : myColors)
	{
		result.emplace_back(name);
	}
	return result;
}

bool ColorManager::CreateImGuiComponents(Crimson::Vector4f& outColor, std::string& outSelection, const std::string& aComboName)
{
	bool result = false;
	bool openPopup = false;
	if (ImGui::BeginCombo(aComboName.c_str(), outSelection.c_str()))
	{
		for (auto& [name, color] : myColors)
		{
			const bool isSelected = (name == outSelection);
			if (ImGui::Selectable(name.c_str(), isSelected))
			{
				outSelection = name;
				outColor = color;
				result = true;
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		if (ImGui::Selectable("<Add Color>", false, ImGuiSelectableFlags_DontClosePopups))
		{
			openPopup = true;			
		}

		if (openPopup)
		{
			ImGui::OpenPopup("Add Color");
		}

		if (ImGui::BeginPopupModal("Add Color"))
		{
			static std::string name;
			static Crimson::Vector4f color = {0.f, 0.f, 0.f, 1.f};

			ImGui::ColorEdit4("Color", &color.x);
			if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				myColors.emplace(name, color);
				outSelection = name;
				outColor = color;
				result = true;
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Save"))
			{
				myColors.emplace(name, color);
				outSelection = name;
				outColor = color;
				result = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::Text("Save Preferences to keep added Colors");
			ImGui::EndPopup();
		}
		ImGui::EndCombo();		
	}

	return result;
}
