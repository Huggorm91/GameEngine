#include "AssetManager.pch.h"
#include "PerspectiveCameraComponent.h"
#include "Engine/Engine.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/GfxCmd_SetFrameBuffer.h"
#include "AssetManager/Assets/GameObject.h"

PerspectiveCameraComponent::PerspectiveCameraComponent() :
	Component(ComponentType::PerspectiveCamera),
	myHasChanged(false),
	myNearPlane(),
	myFarPlane(),
	myFoVDegree()
{}

PerspectiveCameraComponent::PerspectiveCameraComponent(float aFoVDegree, float aNearPlane, float aFarPlane) :
	Component(ComponentType::PerspectiveCamera),
	myHasChanged(false),
	myNearPlane(aNearPlane),
	myFarPlane(aFarPlane),
	myFoVDegree(aFoVDegree)
{
	SetHorizontalFOV(myFoVDegree);
	SetPlanes(myNearPlane, myFarPlane);
}

PerspectiveCameraComponent::PerspectiveCameraComponent(const Json::Value& aJson) :
	Component(aJson),
	myPosition(aJson["Position"]),
	myRotation(aJson["Rotation"]),
	myNearPlane(aJson["NearPlane"].asFloat()),
	myFarPlane(aJson["FarPlane"].asFloat()),
	myFoVDegree(aJson["FoV"].asFloat())
{
	SetHorizontalFOV(myFoVDegree);
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

	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_SetFrameBuffer>(myView, myProjection, myWorldPosition));
}

void PerspectiveCameraComponent::SetHorizontalFOV(float aDegree)
{
	myFoVDegree = aDegree;
	myProjection.m11 = CalculateFOV(Crimson::DegreeToRadian(aDegree));
	myProjection.m22 = myProjection.m11 * (static_cast<float>(Engine::GetWindowSize().x) / static_cast<float>(Engine::GetWindowSize().y));
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
	Crimson::Vector2f windowSize = Engine::GetWindowSize();

	float halfWidth = windowSize.x * 0.5f;
	screenPosition.x = Crimson::Lerp(halfWidth, windowSize.x, aClipSpacePosition.x);

	float halfHeight = windowSize.y * 0.5f;
	screenPosition.y = Crimson::Lerp(halfHeight, windowSize.y, aClipSpacePosition.y);

	anOutScale = 1.f - aClipSpacePosition.z;
	return screenPosition;
}

void PerspectiveCameraComponent::SetPosition(const Crimson::Vector3f& aPosition)
{
	myPosition = aPosition;
	myHasChanged = true;
}

void PerspectiveCameraComponent::SetRotation(const Crimson::Vector3f& aRotation)
{
	myRotation = DegreeToRadian(aRotation);
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

const Crimson::Vector3f& PerspectiveCameraComponent::GetRotationRadian() const
{
	return myRotation;
}

Crimson::Vector3f PerspectiveCameraComponent::GetRotationDegree() const
{
	return RadianToDegree(myRotation);
}

void PerspectiveCameraComponent::TransformHasChanged() const
{
	const_cast<PerspectiveCameraComponent*>(this)->UpdateTransform();
}

Json::Value PerspectiveCameraComponent::ToJson() const
{
	Json::Value result = Component::ToJson();
	result["FoV"] = myFoVDegree;
	result["NearPlane"] = myNearPlane;
	result["FarPlane"] = myFarPlane;
	result["Position"] = static_cast<Json::Value>(myPosition);
	result["Rotation"] = static_cast<Json::Value>(myRotation);

	return result;
}

std::string PerspectiveCameraComponent::ToString() const
{
	return "PerspectiveCamera";
}

const PerspectiveCameraComponent* PerspectiveCameraComponent::GetTypePointer() const
{
	return this;
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