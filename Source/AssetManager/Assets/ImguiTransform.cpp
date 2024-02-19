#include "AssetManager.pch.h"
#include "ImguiTransform.h"
#include "ModelViewer/ModelViewer.h"
#include "ImGui/imgui.h"
#include "ModelViewer/Commands/EditCmd_ChangeTransform.h"
#include "ModelViewer/Commands/EditCmd_ChangeMultipleGameObjects.h"

void CreateImGuiComponents(Transform& aTransform)
{
	ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
	if (ImGui::TreeNode("Transform"))
	{
		auto position = aTransform.GetPosition();
		if (ImGui::DragFloat3("Position", &position.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeTransform>(aTransform.myPosition, position, aTransform.myHasChanged));
		}

		auto rotation = aTransform.GetRotation();
		if (ImGui::DragFloat3("Rotation", &rotation.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeTransform>(aTransform.myRotation, rotation, aTransform.myHasChanged));
		}

		auto scale = aTransform.GetScale();
		if (ImGui::DragFloat3("Scale", &scale.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeTransform>(aTransform.myScale, scale, aTransform.myHasChanged));
		}
		ImGui::TreePop();
	}
}

bool CreateMultipleSelectionImGuiComponents(Transform& aTransform)
{
	bool hasChanged = false;
	ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
	if (ImGui::TreeNode("Transform"))
	{
		auto position = aTransform.GetPosition();
		if (ImGui::DragFloat3("Position", &position.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeMultipleGameObjects>(position - aTransform.GetPosition(), EditCmd_ChangeMultipleGameObjects::TransformType::Position, &aTransform));
			aTransform.SetPosition(position);
			hasChanged = true;
		}

		auto rotation = aTransform.GetRotation();
		if (ImGui::DragFloat3("Rotation", &rotation.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeMultipleGameObjects>(rotation - aTransform.GetRotation(), EditCmd_ChangeMultipleGameObjects::TransformType::Rotation, &aTransform));
			aTransform.SetRotation(rotation);
			hasChanged = true;
		}

		auto scale = aTransform.GetScale();
		if (ImGui::DragFloat3("Scale", &scale.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeMultipleGameObjects>(scale - aTransform.GetScale(), EditCmd_ChangeMultipleGameObjects::TransformType::Scale, &aTransform));
			aTransform.SetScale(scale);
			hasChanged = true;
		}
		ImGui::TreePop();
	}
	return hasChanged;
}
