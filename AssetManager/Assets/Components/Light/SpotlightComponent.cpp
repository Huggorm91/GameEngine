#include "AssetManager.pch.h"
#include "SpotlightComponent.h"
#include "Assets/GameObject.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/Light/LitCmd_AddSpotlight.h"

SpotlightComponent::SpotlightComponent() : myRadius(), myIntensity(), myInnerAngle(), myOuterAngle(), myConeIntensityDifference(), myPosition(), myLightDirection(), myColor()
{
}

SpotlightComponent::SpotlightComponent(float aRadius, float anIntensity, float anInnerAngle, float anOuterAngle, float aDifference, const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aPosition, const CommonUtilities::Vector3f& aColor) :
	myRadius(aRadius), myIntensity(anIntensity), myInnerAngle(anInnerAngle), myOuterAngle(anOuterAngle), myConeIntensityDifference(aDifference), myPosition(aPosition), myLightDirection(aDirection), myColor(aColor)
{
}

void SpotlightComponent::Init(float aRadius, float anIntensity, float anInnerAngle, float anOuterAngle, float aDifference, const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aPosition, const CommonUtilities::Vector3f& aColor)
{
	myRadius = aRadius;
	myIntensity = anIntensity;
	myInnerAngle = anInnerAngle;
	myOuterAngle = anOuterAngle;
	myConeIntensityDifference = aDifference;
	myPosition = aPosition;
	myLightDirection = aDirection;
	myColor = aColor;
}

void SpotlightComponent::Update()
{
	if (!myIsActive)
	{
		return;
	}

	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<LitCmd_AddSpotlight>(*this));
}

void SpotlightComponent::SetRadius(float aRadius)
{
	myRadius = aRadius;
}

void SpotlightComponent::SetIntensity(float anIntensity)
{
	myIntensity = anIntensity;
}

void SpotlightComponent::SetInnerAngle(float anAngle)
{
	myInnerAngle = anAngle;
}

void SpotlightComponent::SetOuterAngle(float anAngle)
{
	myOuterAngle = anAngle;
}

void SpotlightComponent::SetConeIntensityDifference(float aDifference)
{
	myConeIntensityDifference = aDifference;
}

void SpotlightComponent::SetPosition(const CommonUtilities::Vector3f& aPosition)
{
	myPosition = aPosition;
}

void SpotlightComponent::SetLightDirection(const CommonUtilities::Vector3f& aDirection)
{
	myLightDirection = aDirection;
}

void SpotlightComponent::SetColor(const CommonUtilities::Vector3f& aColor)
{
	myColor = aColor;
}

const SpotlightComponent* SpotlightComponent::GetTypePointer() const
{
	return this;
}

float SpotlightComponent::GetRadius() const
{
	return myRadius;
}

float SpotlightComponent::GetIntensity() const
{
	return myIntensity;
}

float SpotlightComponent::GetInnerAngle() const
{
	return myInnerAngle;
}

float SpotlightComponent::GetOuterAngle() const
{
	return myOuterAngle;
}

float SpotlightComponent::GetConeIntensityDifference() const
{
	return myConeIntensityDifference;
}

const CommonUtilities::Vector3f& SpotlightComponent::GetPosition() const
{
	return myPosition;
}

const CommonUtilities::Vector3f& SpotlightComponent::GetLightDirection() const
{
	return myLightDirection;
}

const CommonUtilities::Vector3f& SpotlightComponent::GetColor() const
{
	return myColor;
}

CommonUtilities::Matrix4x4f SpotlightComponent::GetTransform() const
{
	assert(myParent != nullptr && "PointlightComponent is not Initialized!");
	return myParent->GetTransformMatrix() * CommonUtilities::Matrix4x4f::CreateTranslationMatrix(myPosition);
}

