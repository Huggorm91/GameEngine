#include "AssetManager.pch.h"
#include "Transform.h"
#include "Math/Conversions.hpp"
#include "Json/JsonVector.hpp"

#ifndef _RETAIL
#include "ModelViewer/Core/ModelViewer.h"
#include "ImGui/imgui.h"
#include "ModelViewer/Core/Commands/EditCmd_ChangeTransform.h"
#include "ModelViewer/Core/Commands/EditCmd_ChangeMultipleGameObjects.h"
#endif // !_RETAIL

Transform::Transform() : myPosition(), myRotation(), myScale(1.f, 1.f, 1.f), myHasChanged(false), myTransform(), myParent(nullptr), myWorldPosition()
{
}

Transform::Transform(const Json::Value& aJson) : myPosition(aJson["Position"]), myRotation(aJson["Rotation"]), myScale(aJson["Scale"]), myHasChanged(true), myTransform(), myParent(nullptr), myWorldPosition()
{
}

Transform::Transform(const Crimson::Vector3f& aPosition, const Crimson::Vector3f& aRotation, const Crimson::Vector3f& aScale) :
	myPosition(aPosition), myRotation(aRotation), myScale(aScale), myHasChanged(false), myTransform(), myParent(nullptr), myWorldPosition()
{
	UpdateTransform();
}

Transform::Transform(const Transform& aTransform) : myPosition(aTransform.myPosition), myRotation(aTransform.myRotation), myScale(aTransform.myScale), myHasChanged(aTransform.myHasChanged),
myTransform(aTransform.myTransform), myParent(nullptr), myWorldPosition(aTransform.myWorldPosition)
{
}

Transform::Transform(Transform&& aTransform) noexcept : myPosition(aTransform.myPosition), myRotation(aTransform.myRotation), myScale(aTransform.myScale), myHasChanged(aTransform.myHasChanged),
myTransform(aTransform.myTransform), myParent(nullptr), myWorldPosition(aTransform.myWorldPosition)
{
}

Transform& Transform::operator=(const Transform& aTransform)
{
	myPosition = aTransform.myPosition;
	myRotation = aTransform.myRotation;
	myScale = aTransform.myScale;
	myHasChanged = aTransform.myHasChanged;
	myTransform = aTransform.myTransform;
	myParent = nullptr;
	myWorldPosition = aTransform.myWorldPosition;
	return *this;
}

Transform& Transform::operator=(Transform&& aTransform) noexcept
{
	myPosition = aTransform.myPosition;
	myRotation = aTransform.myRotation;
	myScale = aTransform.myScale;
	myHasChanged = aTransform.myHasChanged;
	myTransform = aTransform.myTransform;
	myParent = nullptr;
	myWorldPosition = aTransform.myWorldPosition;
	return *this;
}

Transform& Transform::operator+=(const Transform& aTransform)
{
	myPosition += aTransform.myPosition;
	myRotation += aTransform.myRotation;
	myScale *= aTransform.myScale;
	myHasChanged = true;
	return *this;
}

Transform& Transform::operator-=(const Transform& aTransform)
{
	myPosition -= aTransform.myPosition;
	myRotation -= aTransform.myRotation;
	myScale /= aTransform.myScale;
	myHasChanged = true;
	return *this;
}

Transform Transform::operator+(const Transform& aTransform) const
{
	Transform result = *this;
	result += aTransform;
	return result;
}

Transform Transform::operator-(const Transform& aTransform) const
{
	Transform result = *this;
	result -= aTransform;
	return result;
}

void Transform::SetPosition(const Crimson::Vector3f& aPosition)
{
	myPosition = aPosition;
	myHasChanged = true;
}

const Crimson::Vector3f& Transform::GetPosition() const
{
	return myPosition;
}

void Transform::SetRotation(const Crimson::Vector3f& aRotation)
{
	myRotation = aRotation;
	myHasChanged = true;
}

const Crimson::Vector3f& Transform::GetRotation() const
{
	return myRotation;
}

void Transform::SetScale(const Crimson::Vector3f& aScale)
{
	myScale = aScale;
	myHasChanged = true;
}

const Crimson::Vector3f& Transform::GetScale() const
{
	return myScale;
}

const Crimson::Vector4f& Transform::GetWorldPosition() const
{
	if (myHasChanged)
	{
		const_cast<Transform*>(this)->UpdateTransform();
	}
	return myWorldPosition;
}

const Crimson::Matrix4x4f& Transform::GetTransformMatrix() const
{
	if (myHasChanged || (myParent && myParent->myHasChanged))
	{
		const_cast<Transform*>(this)->UpdateTransform();
	}
	return myTransform;
}

const Transform* Transform::GetParent() const
{
	return myParent;
}

Transform* Transform::GetParent()
{
	return myParent;
}

void Transform::SetHasChanged(bool aState)
{
	myHasChanged = aState;
}

bool Transform::HasChanged() const
{
	return myHasChanged;
}

void Transform::SetParent(Transform* aParent)
{
	myParent = aParent;
	UpdateTransform();
}

void Transform::RemoveParent()
{
	myParent = nullptr;
	UpdateTransform();
}

void Transform::Update()
{
	if (myHasChanged || (myParent && myParent->myHasChanged))
	{
		const_cast<Transform*>(this)->UpdateTransform();
	}
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

bool Transform::CreateMultipleSelectionImGuiComponents(const std::string&)
{
#ifndef _RETAIL
	bool hasChanged = false;
	ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
	if (ImGui::TreeNode("Transform"))
	{
		auto position = myPosition;
		if (ImGui::DragFloat3("Position", &position.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeMultipleGameObjects>(position - myPosition, EditCmd_ChangeMultipleGameObjects::TransformType::Position, this));
			myPosition = position;
			myHasChanged = true;
			hasChanged = true;
		}

		auto rotation = myRotation;
		if (ImGui::DragFloat3("Rotation", &rotation.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeMultipleGameObjects>(rotation - myRotation, EditCmd_ChangeMultipleGameObjects::TransformType::Rotation, this));
			myRotation = rotation;
			myHasChanged = true;
			hasChanged = true;
		}

		auto scale = myScale;
		if (ImGui::DragFloat3("Scale", &scale.x))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeMultipleGameObjects>(scale - myScale, EditCmd_ChangeMultipleGameObjects::TransformType::Scale, this));
			myScale = scale;
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

void Transform::Serialize(std::ostream& aStream) const
{
	myPosition.Serialize(aStream);
	myRotation.Serialize(aStream);
	myScale.Serialize(aStream);
}

void Transform::Deserialize(std::istream& aStream)
{
	myPosition.Deserialize(aStream);
	myRotation.Deserialize(aStream);
	myScale.Deserialize(aStream);
}

void Transform::UpdateTransform()
{
	myTransform = GetTotalTransform();
	myWorldPosition = myTransform * Crimson::Vector4f{ 0.f, 0.f, 0.f, 1.f };
	myHasChanged = false;
}

Crimson::Matrix4x4f Transform::GetTransform() const
{
	return Crimson::Matrix4x4f::CreateScaleMatrix(myScale) *
		Crimson::Matrix4x4f::CreateRotationMatrix(Crimson::DegreeToRadian(myRotation)) *
		Crimson::Matrix4x4f::CreateTranslationMatrix(myPosition);
}

Crimson::Matrix4x4f Transform::GetTotalTransform() const
{
	if (myParent)
	{
		return GetTransform() * myParent->GetTotalTransform();
	}
	else
	{
		return GetTransform();
	}
}

Crimson::Vector3f Transform::GetTotalPosition() const
{
	if (myParent)
	{
		return myParent->GetTotalPosition() + myPosition;
	}
	else
	{
		return myPosition;
	}
}

Crimson::Vector3f Transform::GetTotalRotation() const
{
	if (myParent)
	{
		return myParent->GetTotalRotation() + myRotation;
	}
	else
	{
		return myRotation;
	}
}

Crimson::Vector3f Transform::GetTotalScale() const
{
	if (myParent)
	{
		return myParent->GetTotalScale() * myScale;
	}
	else
	{
		return myScale;
	}
}
