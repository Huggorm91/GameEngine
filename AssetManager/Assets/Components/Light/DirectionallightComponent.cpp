#include "AssetManager.pch.h"
#include "DirectionallightComponent.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetDirectionallight.h"

DirectionallightComponent::DirectionallightComponent() : myInvertedLightDirection(), myColor(1.f, 1.f, 1.f), myIntensity(1.f)
{
}

DirectionallightComponent::DirectionallightComponent(const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aColor, float anIntensity): myInvertedLightDirection(-aDirection), myColor(aColor), myIntensity(anIntensity)
{
}

void DirectionallightComponent::Update()
{
	if (!myIsActive)
	{
		return;
	}

	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<LitCmd_SetDirectionallight>(*this));
}

void DirectionallightComponent::SetLightDirection(const CommonUtilities::Vector3f& aDirection)
{
	myInvertedLightDirection = -aDirection;
}

void DirectionallightComponent::SeColor(const CommonUtilities::Vector3f& aColor)
{
	myColor = aColor;
}

void DirectionallightComponent::SetIntensity(float anIntensity)
{
	myIntensity = anIntensity;
}

CommonUtilities::Vector3f DirectionallightComponent::GetLightDirection() const
{
	return -myInvertedLightDirection;
}

const CommonUtilities::Vector3f& DirectionallightComponent::GetInvertedLightDirection() const
{
	return myInvertedLightDirection;
}

const CommonUtilities::Vector3f& DirectionallightComponent::GetColor() const
{
	return myColor;
}

float DirectionallightComponent::GetIntensity() const
{
	return myIntensity;
}

const DirectionallightComponent* DirectionallightComponent::GetTypePointer() const
{
	return this;
}
