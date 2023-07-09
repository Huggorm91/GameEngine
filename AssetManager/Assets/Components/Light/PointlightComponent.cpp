#include "AssetManager.pch.h"
#include "PointlightComponent.h"
#include "Assets/GameObject.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/Light/LitCmd_AddPointlight.h"

PointlightComponent::PointlightComponent(): myRadius(), myIntensity(), myPosition(), myColor(1.f, 1.f, 1.f)
{
}

PointlightComponent::PointlightComponent(float aRadius, float anIntensity, const CommonUtilities::Vector3f& aPosition, const CommonUtilities::Vector3f& aColor) : myRadius(aRadius), myIntensity(anIntensity), myPosition(aPosition), myColor(aColor)
{
}

void PointlightComponent::Init(float aRadius, float anIntensity, const CommonUtilities::Vector3f& aPosition, const CommonUtilities::Vector3f& aColor)
{
	myRadius = aRadius;
	myIntensity = anIntensity;
	myPosition = aPosition;
	myColor = aColor;
}

void PointlightComponent::Update()
{
	if (!myIsActive)
	{
		return;
	}

	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<LitCmd_AddPointlight>(*this));
}

void PointlightComponent::SetRadius(float aRadius)
{
	myRadius = aRadius;
}

void PointlightComponent::SetIntensity(float anIntensity)
{
	myIntensity = anIntensity;
}

void PointlightComponent::SetPosition(const CommonUtilities::Vector3f& aPosition)
{
	myPosition = aPosition;
}

void PointlightComponent::SetColor(const CommonUtilities::Vector3f& aColor)
{
	myColor = aColor;
}

float PointlightComponent::GetRadius() const
{
	return myRadius;
}

float PointlightComponent::GetIntensity() const
{
	return myIntensity;
}

const CommonUtilities::Vector3f& PointlightComponent::GetPosition() const
{
	return myPosition;
}

const CommonUtilities::Vector3f& PointlightComponent::GetColor() const
{
	return myColor;
}

CommonUtilities::Matrix4x4f PointlightComponent::GetTransform() const
{
	assert(myParent != nullptr && "PointlightComponent is not Initialized!");
	return myParent->GetTransformMatrix() * CommonUtilities::Matrix4x4f::CreateTranslationMatrix(myPosition);
}

const PointlightComponent* PointlightComponent::GetTypePointer() const
{
	return this;
}
