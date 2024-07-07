#include "pch.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "ImNodeEd_Combo.h"
#include "ScriptGraphEditorTypes.h"
#include "String/StringFunctions.h"

extern "C" void __MuninGraph_AutoRegEditorTypes() {  }

IMPLEMENT_EDITOR_TYPE(bool, Bool)

bool ScriptGraphEditorType_Bool::TypeEditWidget(std::string_view aUniqueName, const TypedDataContainer& aDataContainer) const
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
	ImGui::Checkbox(aUniqueName.data(), static_cast<bool*>(*aDataContainer));
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		return true;
	}

	return false;
}

std::string ScriptGraphEditorType_Bool::ToString(const TypedDataContainer& aDataContainer) const
{
	bool f = 0;
	aDataContainer.TryGet(f);
	if (f)
	{
		return "True";
	}
	return "False";
};

IMPLEMENT_EDITOR_TYPE(int, Int);

bool ScriptGraphEditorType_Int::TypeEditWidget(std::string_view aUniqueName, const TypedDataContainer& aDataContainer) const
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
	ImGui::SetNextItemWidth(150);
	ImGui::InputInt(aUniqueName.data(), static_cast<int*>(*aDataContainer), 0, 0);
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		return true;
	}

	return false;
}

std::string ScriptGraphEditorType_Int::ToString(const TypedDataContainer& aDataContainer) const
{
	int f = 0;
	aDataContainer.TryGet(f);
	return std::to_string(f);
};

IMPLEMENT_EDITOR_TYPE(unsigned, GameObjectID);

bool ScriptGraphEditorType_GameObjectID::TypeEditWidget(std::string_view aUniqueName, const TypedDataContainer& aDataContainer) const
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
	ImGui::SetNextItemWidth(150);
	int value = *static_cast<unsigned*>(*aDataContainer);
	if (ImGui::InputInt(aUniqueName.data(), &value))
	{
		*static_cast<unsigned*>(*aDataContainer) = std::clamp(value, 0, INT_MAX);
	}

	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		return true;
	}

	return false;
}

std::string ScriptGraphEditorType_GameObjectID::ToString(const TypedDataContainer& aDataContainer) const
{
	unsigned f = 0;
	aDataContainer.TryGet(f);
	return std::to_string(f);
};

IMPLEMENT_EDITOR_TYPE(float, Float)

bool ScriptGraphEditorType_Float::TypeEditWidget(std::string_view aUniqueName, const TypedDataContainer& aDataContainer) const
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
	ImGui::SetNextItemWidth(150);
	ImGui::InputFloat(aUniqueName.data(), static_cast<float*>(*aDataContainer), 0, 0, "%.1f");
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		return true;
	}

	return false;
}

std::string ScriptGraphEditorType_Float::ToString(const TypedDataContainer& aDataContainer) const
{
	float f = 0;
	aDataContainer.TryGet(f);
	return std::to_string(f);
};

IMPLEMENT_EDITOR_TYPE(std::string, String)

bool ScriptGraphEditorType_String::TypeEditWidget(std::string_view aUniqueName, const TypedDataContainer& aDataContainer) const
{
	ImGui::NewLine();
	ImGui::PushItemWidth(150);
	ImGui::InputText(aUniqueName.data(), static_cast<std::string*>(*aDataContainer), ImGuiInputTextFlags_EnterReturnsTrue);
	bool result = false;
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		result = true;
	}
	ImGui::PopItemWidth();
	return result;
}

std::string ScriptGraphEditorType_String::ToString(const TypedDataContainer& aDataContainer) const
{
	return *static_cast<const std::string*>(*aDataContainer);
}

IMPLEMENT_EDITOR_TYPE(Crimson::Vector3f, Vector3)

std::string ScriptGraphEditorType_Vector3::ToString(const TypedDataContainer& aDataContainer) const
{
	Crimson::Vector3f f;
	aDataContainer.TryGet(f);
	return f.ToString();
}

IMPLEMENT_EDITOR_TYPE(Crimson::eKey, KeyEnum)

bool ScriptGraphEditorType_KeyEnum::TypeEditWidget(std::string_view aUniqueName, const TypedDataContainer& aDataContainer) const
{
	ImGui::NewLine();
	ImGui::PushItemWidth(150);
	ImGui::PushID(std::string(aUniqueName).c_str());

	Crimson::eKey* value = static_cast<Crimson::eKey*>(*aDataContainer);
	
	if (ImNodeEd::BeginNodeCombo("", Crimson::KeyToString(*value).c_str()))
	{
		static std::string localSearch;
		ImGui::InputText("##ctxtMenuSearch", &localSearch);

		std::string currentString;
		Crimson::eKey current = Crimson::eKey::None;
		for (int index = 0; index < 255; index++)
		{
			current = static_cast<Crimson::eKey>(index);
			currentString = Crimson::ToLower(Crimson::KeyToString(current));
			if (currentString == "unknown")
			{
				continue;
			}

			if (!localSearch.empty() && currentString.find(Crimson::ToLower(localSearch)) == std::string::npos)
			{
				continue;
			}

			const bool isSelected = *value == current;
			if (ImGui::Selectable(Crimson::KeyToString(current).c_str(), isSelected))
			{
				*value = current;
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImNodeEd::EndNodeCombo();
	}
	ImGui::PopID();

	bool result = false;
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		result = true;
	}
	ImGui::PopItemWidth();
	return result;
}

std::string ScriptGraphEditorType_KeyEnum::ToString(const TypedDataContainer& aDataContainer) const
{
	Crimson::eKey f;
	aDataContainer.TryGet(f);
	return Crimson::KeyToString(f);
}

IMPLEMENT_EDITOR_TYPE(void*, Internal_VoidPtr);
