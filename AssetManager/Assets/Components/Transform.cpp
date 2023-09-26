#include "AssetManager.pch.h"
#include "Transform.h"
#include <Conversions.hpp>
#include <JsonVector.hpp>

#ifndef _RETAIL
#include "ModelViewer/Core/ModelViewer.h"
#include "ThirdParty/DearImGui/ImGui/imgui.h"
#include "ModelViewer/Core/Commands/EditCmd_ChangeTransform.h"
#endif // !_RETAIL

Transform::Transform(): myPosition(), myRotation(), myScale(1.f, 1.f, 1.f), myHasChanged(false), myTransform()
{
}

Transform::Transform(const Json::Value& aJson): myPosition(aJson["Position"]), myRotation(aJson["Rotation"]), myScale(aJson["Scale"]), myHasChanged(true), myTransform()
{
}

Transform::Transform(const CommonUtilities::Vector3f& aPosition, const CommonUtilities::Vector3f& aRotation, const CommonUtilities::Vector3f& aScale):
	myPosition(aPosition), myRotation(aRotation), myScale(aScale), myHasChanged(false), myTransform()
{
	UpdateTransform();
}

void Transform::SetPosition(const CommonUtilities::Vector3f& aPosition)
{
	myPosition = aPosition;
	myHasChanged = true;
}

const CommonUtilities::Vector3f& Transform::GetPosition() const
{
	return myPosition;
}

void Transform::SetRotation(const CommonUtilities::Vector3f& aRotation)
{
	myRotation = aRotation;
	myHasChanged = true;
}

const CommonUtilities::Vector3f& Transform::GetRotation() const
{
	return myRotation;
}

void Transform::SetScale(const CommonUtilities::Vector3f& aScale)
{
	myScale = aScale;
	myHasChanged = true;
}

const CommonUtilities::Vector3f& Transform::GetScale() const
{
	return myScale;
}

const CommonUtilities::Vector3f& Transform::GetWorldPosition() const
{
	return myWorldPosition;
}

const CommonUtilities::Matrix4x4f& Transform::GetTransformMatrix() const
{
	if (myHasChanged)
	{
		const_cast<Transform*>(this)->UpdateTransform();
	}
	return myTransform;
}

bool Transform::HasChanged() const
{
	return myHasChanged;
}

void Transform::CreateImGuiComponents(const std::string&)
{
#ifndef _RETAIL
	ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
	if (ImGui::TreeNode("Transform"))
	{
		auto position = myPosition;
		if (ImGui::DragFloat3("Position", &position.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeTransform>(myPosition, position, myHasChanged));
		}

		auto rotation = myRotation;
		if (ImGui::DragFloat3("Rotation", &rotation.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeTransform>(myRotation, rotation, myHasChanged));
		}

		auto scale = myScale;
		if (ImGui::DragFloat3("Scale", &scale.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeTransform>(myScale, scale, myHasChanged));
		}
		ImGui::TreePop();
	}	
#endif // !_RETAIL
}

bool Transform::CreateMultipleSelectionImGuiComponents(const std::string& aWindowName)
{
#ifndef _RETAIL
	bool hasChanged = false;
	ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
	if (ImGui::TreeNode("Transform"))
	{
		if (ImGui::DragFloat3("Position", &myPosition.x))
		{
			myHasChanged = true;
			hasChanged = true;
		}

		if (ImGui::DragFloat3("Rotation", &myRotation.x))
		{
			myHasChanged = true;
			hasChanged = true;
		}

		if (ImGui::DragFloat3("Scale", &myScale.x))
		{
			myHasChanged = true;
			hasChanged = true;
		}
		ImGui::TreePop();
	}
	return hasChanged;
#endif // !_RETAIL
}

Json::Value Transform::ToJson() const
{
	Json::Value result;
	result["Position"] = static_cast<Json::Value>(myPosition);
	result["Rotation"] = static_cast<Json::Value>(myRotation);
	result["Scale"] = static_cast<Json::Value>(myScale);
	return result;
}

void Transform::UpdateTransform()
{
	myHasChanged = false;
	myTransform = CommonUtilities::Matrix4x4f::CreateScaleMatrix(myScale) *
		CommonUtilities::Matrix4x4f::CreateRotationMatrix(CommonUtilities::DegreeToRadian(myRotation)) *
		CommonUtilities::Matrix4x4f::CreateTranslationMatrix(myPosition);
	myWorldPosition = myTransform * CommonUtilities::Vector4f{ myPosition, 1.f };
}
