#include "MuninGraphEditor.pch.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "ScriptGraphEditorTypes.h"

extern "C" void __MuninGraph_AutoRegEditorTypes() {  }

IMPLEMENT_EDITOR_TYPE(float, Float)

bool ScriptGraphEditorType_Float::TypeEditWidget(std::string_view aUniqueName, const TypedDataContainer& aDataContainer) const
{
	const float y = ImGui::GetCursorPosY();
	ImGui::SetCursorPosY(y - 2);
	const ImVec2 inputSize = ImGui::CalcTextSize("0.0000");
	ImGui::SetNextItemWidth(inputSize.x);
	ImGui::InputFloat(aUniqueName.data(), static_cast<float*>(*aDataContainer), 0, 0, "%.1f");
	if(ImGui::IsItemDeactivatedAfterEdit())
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
	if(ImGui::IsItemDeactivatedAfterEdit())
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

IMPLEMENT_EDITOR_TYPE(void*, Internal_VoidPtr);
