#include "AssetManager.pch.h"
#include "Transform.h"
#include <Conversions.hpp>

Transform::Transform(): myPosition(), myRotation(), myScale(1.f, 1.f, 1.f), myHasChanged(false), myTransform()
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

void Transform::UpdateTransform()
{
	myHasChanged = false;
	myTransform = CommonUtilities::Matrix4x4f::CreateScaleMatrix(myScale) *
		CommonUtilities::Matrix4x4f::CreateRotationMatrix(CommonUtilities::DegreeToRadian(myRotation)) *
		CommonUtilities::Matrix4x4f::CreateTranslationMatrix(myPosition);
	myWorldPosition = myTransform * CommonUtilities::Vector4f{ myPosition, 1.f };
}
