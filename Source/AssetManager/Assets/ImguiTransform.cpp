#include "AssetManager.pch.h"
#include "ImguiTransform.h"
#ifdef EDITOR
#include "Editor/ModelViewer.h"
#include "Editor/Commands/EditCmd_ChangeTransform.h"
#include "Editor/Commands/EditCmd_ChangeMultipleGameObjects.h"
#endif // EDITOR

void CreateImGuiComponents(Transform& aTransform)
{
#ifdef EDITOR
	ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
	if (ImGui::TreeNode("Transform"))
	{
		auto position = aTransform.GetPosition();
		if (ImGui::DragFloat3("Position", &position.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeTransform>(aTransform.myPosition, position, aTransform.myHasChangedInternal));
		}

		auto rotation = aTransform.GetRotationDegree();
		if (ImGui::DragFloat3("Rotation", &rotation.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeTransform>(aTransform.myRotation, Crimson::DegreeToRadian(rotation), aTransform.myHasChangedInternal));
		}

		auto scale = aTransform.GetScale();
		if (ImGui::DragFloat3("Scale", &scale.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeTransform>(aTransform.myScale, scale, aTransform.myHasChangedInternal));
		}
		ImGui::TreePop();
	}
#else
	aTransform;
#endif // EDITOR
}

bool CreateImGuiComponentsNoUndo(Transform& aTransform)
{
#ifdef EDITOR
	bool hasChanged = false;
	ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
	if (ImGui::TreeNode("Transform"))
	{
		auto position = aTransform.GetPosition();
		if (ImGui::DragFloat3("Position", &position.x))
		{
			aTransform.SetPosition(position);
			hasChanged = true;
		}

		auto rotation = aTransform.GetRotationDegree();
		if (ImGui::DragFloat3("Rotation", &rotation.x))
		{
			aTransform.SetRotationDegree(rotation);
			hasChanged = true;
		}

		auto scale = aTransform.GetScale();
		if (ImGui::DragFloat3("Scale", &scale.x))
		{
			aTransform.SetScale(scale);
			hasChanged = true;
		}
		ImGui::TreePop();
	}
	return hasChanged;
#else
	aTransform;
	return false;
#endif // EDITOR
}

bool CreateMultipleSelectionImGuiComponents(Transform& aTransform)
{
#ifdef EDITOR
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

		auto rotation = aTransform.GetRotationDegree();
		if (ImGui::DragFloat3("Rotation", &rotation.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeMultipleGameObjects>(rotation - aTransform.GetRotationDegree(), EditCmd_ChangeMultipleGameObjects::TransformType::Rotation, &aTransform));
			aTransform.SetRotationDegree(rotation);
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
#else
	aTransform;
	return false;
#endif // EDITOR
}
