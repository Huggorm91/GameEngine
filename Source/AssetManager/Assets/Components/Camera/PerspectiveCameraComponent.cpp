#include "AssetManager.pch.h"
#include "PerspectiveCameraComponent.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/GfxCmd_SetFrameBuffer.h"
#include "AssetManager/Assets/GameObject.h"

PerspectiveCameraComponent::PerspectiveCameraComponent() :
	Component(ComponentType::PerspectiveCamera),
	myHasChanged(false),
	myIsVerticalFoV(false),
	myNearPlane(),
	myFarPlane(),
	myFoVDegree()
{
}

PerspectiveCameraComponent::PerspectiveCameraComponent(float aFoVDegree, float aNearPlane, float aFarPlane, bool anIsVerticalFoV) :
	Component(ComponentType::PerspectiveCamera),
	myHasChanged(false),
	myIsVerticalFoV(anIsVerticalFoV),
	myNearPlane(aNearPlane),
	myFarPlane(aFarPlane),
	myFoVDegree(aFoVDegree)
{
	if (myIsVerticalFoV)
	{
		SetVerticalFOV(myFoVDegree);
	}
	else
	{
		SetHorizontalFOV(myFoVDegree);
	}
	SetPlanes(myNearPlane, myFarPlane);
}

PerspectiveCameraComponent::PerspectiveCameraComponent(const Json::Value& aJson) :
	Component(aJson),
	myPosition(aJson["Position"]),
	myRotation(aJson["Rotation"]),
	myNearPlane(aJson["NearPlane"].asFloat()),
	myFarPlane(aJson["FarPlane"].asFloat()),
	myFoVDegree(aJson["FoV"].asFloat()),
	myIsVerticalFoV(aJson["IsVerticalFoV"].asBool())
{
	if (myIsVerticalFoV)
	{
		SetVerticalFOV(myFoVDegree);
	}
	else
	{
		SetHorizontalFOV(myFoVDegree);
	}
	SetPlanes(myNearPlane, myFarPlane);

	UpdateTransform();
}

void PerspectiveCameraComponent::Update()
{
	if (!myIsActive)
	{
		return;
	}

	if (myHasChanged)
	{
		UpdateTransform();
	}

	if (myHasChanged)
	{
		UpdateTransform();
	}
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_SetFrameBuffer>(myView, myProjection, myPosition));
}

void PerspectiveCameraComponent::SetHorizontalFOV(float aDegree)
{
	myIsVerticalFoV = false;
	myFoVDegree = aDegree;
	myProjection.m11 = CalculateFOV(Crimson::DegreeToRadian(aDegree));
	Crimson::Vector2f window = GraphicsEngine::Get().GetWindowSize();
	myProjection.m22 = myProjection.m11 * (window.x / window.y);
}

void PerspectiveCameraComponent::SetVerticalFOV(float aDegree)
{
	myIsVerticalFoV = true;
	myFoVDegree = aDegree;
	myProjection.m22 = CalculateFOV(Crimson::DegreeToRadian(aDegree));
	Crimson::Vector2f window = GraphicsEngine::Get().GetWindowSize();
	myProjection.m11 = myProjection.m22 * (window.y / window.x);
}

void PerspectiveCameraComponent::SetPlanes(float aNearPlane, float aFarPlane)
{
	myNearPlane = aNearPlane;
	myFarPlane = aFarPlane;

	const float divideInverse = 1.f / (aFarPlane - aNearPlane);
	myProjection.m33 = aFarPlane * divideInverse;
	myProjection.m43 = -aNearPlane * aFarPlane * divideInverse;

	myProjection.m34 = 1.f;
	myProjection.m44 = 0.f;
}

Crimson::Vector3f PerspectiveCameraComponent::TransformToClipSpace(const Crimson::Vector3f& aPoint) const
{
	Crimson::Vector4f clipSpace = Crimson::Vector4f(aPoint, 1.f) * myView * myProjection;
	clipSpace.x /= clipSpace.w;
	clipSpace.y /= clipSpace.w;
	clipSpace.z /= clipSpace.w;
	return clipSpace;
}

Crimson::Vector2f PerspectiveCameraComponent::ConvertToScreenCoordinates(const Crimson::Vector3f& aClipSpacePosition, float& anOutScale) const
{
	Crimson::Vector2f screenPosition;
	Crimson::Vector2f window = GraphicsEngine::Get().GetWindowSize();
	float halfWidth = window.x * 0.5f;
	screenPosition.x = Crimson::Lerp(halfWidth, window.x, aClipSpacePosition.x);

	float halfHeight = window.y * 0.5f;
	screenPosition.y = Crimson::Lerp(halfHeight, window.y, aClipSpacePosition.y);

	anOutScale = 1.f - aClipSpacePosition.z;
	return screenPosition;
}

void PerspectiveCameraComponent::AddToPosition(const Crimson::Vector3f& aPosition)
{
	myPosition += aPosition;
	myHasChanged = true;
}

void PerspectiveCameraComponent::SetPosition(const Crimson::Vector3f& aPosition)
{
	myPosition = aPosition;
	myHasChanged = true;
}

const Crimson::Vector3f& PerspectiveCameraComponent::GetPosition() const
{
	return myPosition;
}

const Crimson::Vector3f& PerspectiveCameraComponent::GetWorldPosition() const
{
	return myWorldPosition;
}

void PerspectiveCameraComponent::SetRotation(const Crimson::Vector3f& aRotation)
{
	myRotation = DegreeToRadian(aRotation);
	myHasChanged = true;
}

void PerspectiveCameraComponent::SetRadianRotation(const Crimson::Vector3f& aRotation)
{
	myRotation = aRotation;
	myHasChanged = true;
}

Crimson::Vector3f PerspectiveCameraComponent::GetRotation() const
{
	return RadianToDegree(myRotation);
}

const Crimson::Vector3f& PerspectiveCameraComponent::GetRadianRotation() const
{
	return myRotation;
}

void PerspectiveCameraComponent::TransformHasChanged() const
{
	const_cast<PerspectiveCameraComponent*>(this)->UpdateTransform();
}

Json::Value PerspectiveCameraComponent::ToJson() const
{
	Json::Value result = Component::ToJson();
	result["IsVerticalFoV"] = myIsVerticalFoV;
	result["FoV"] = myFoVDegree;
	result["NearPlane"] = myNearPlane;
	result["FarPlane"] = myFarPlane;
	result["Position"] = static_cast<Json::Value>(myPosition);
	result["Rotation"] = static_cast<Json::Value>(myRotation);

	return result;
}

void PerspectiveCameraComponent::UpdateTransform()
{
	if (myParent)
	{
		myWorldPosition = GetParentTransform()->GetTransformMatrix() * Crimson::Vector4f(myPosition, 1.f);
	}

	myView = Crimson::Matrix4x4f::CreateRotationAroundX(myRotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(myRotation.y);

	myView.m41 = myWorldPosition.x;
	myView.m42 = myWorldPosition.y;
	myView.m43 = myWorldPosition.z;

	myView = myView.GetInverse();
	myHasChanged = false;
}

float PerspectiveCameraComponent::CalculateFOV(float aRadian) const
{
	return 1 / tan(aRadian * 0.5f);
}

const Crimson::Matrix4x4f& PerspectiveCameraComponent::GetView() const
{
	return myView;
}

const Crimson::Matrix4x4f& PerspectiveCameraComponent::GetProjection() const
{
	return myProjection;
}