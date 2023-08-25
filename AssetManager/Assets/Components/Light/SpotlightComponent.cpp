#include "AssetManager.pch.h"
#include "SpotlightComponent.h"
#include "Assets/GameObject.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/Light/LitCmd_AddSpotlight.h"

SpotlightComponent::SpotlightComponent() : myRange(), myIntensity(), myInnerAngle(), myOuterAngle(), myConeIntensityDifference(), myPosition(), myLightDirection(), myColor(), myCastShadows(false), myShadowMap(nullptr)
{
}

SpotlightComponent::SpotlightComponent(float aRange, float anIntensity, float anInnerAngle, float anOuterAngle, float aDifference, const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aPosition, const CommonUtilities::Vector3f& aColor, bool aCastShadows) :
	myRange(aRange), myIntensity(anIntensity), myInnerAngle(CommonUtilities::DegreeToRadian(anInnerAngle)), myOuterAngle(CommonUtilities::DegreeToRadian(anOuterAngle)), myConeIntensityDifference(aDifference), myPosition(aPosition), myLightDirection(aDirection.GetNormalized()), myColor(aColor), myCastShadows(aCastShadows), myShadowMap(nullptr)
{
	if (aCastShadows)
	{
		CreateShadowMap();
	}
}

SpotlightComponent::SpotlightComponent(const SpotlightComponent& aLight) : myRange(aLight.myRange), myIntensity(aLight.myIntensity), myInnerAngle(aLight.myInnerAngle), myOuterAngle(aLight.myOuterAngle), myConeIntensityDifference(aLight.myConeIntensityDifference), 
myPosition(aLight.myPosition), myLightDirection(aLight.myLightDirection), myColor(aLight.myColor), myCastShadows(aLight.myCastShadows), myShadowMap(nullptr)
{
	if (myCastShadows)
	{
		CreateShadowMap();
	}
}

SpotlightComponent& SpotlightComponent::operator=(const SpotlightComponent& aLight)
{
	myRange = aLight.myRange;
	myIntensity = aLight.myIntensity;
	myInnerAngle = aLight.myInnerAngle;
	myOuterAngle = aLight.myOuterAngle;
	myConeIntensityDifference = aLight.myConeIntensityDifference;
	myPosition = aLight.myPosition;
	myLightDirection = aLight.myLightDirection;
	myColor = aLight.myColor;
	myCastShadows = aLight.myCastShadows;

	if (myCastShadows && myShadowMap == nullptr)
	{
		CreateShadowMap();
	}
	return *this;
}

void SpotlightComponent::Init(float aRange, float anIntensity, float anInnerAngle, float anOuterAngle, float aDifference, const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aPosition, const CommonUtilities::Vector3f& aColor, bool aCastShadows)
{
	myRange = aRange;
	myIntensity = anIntensity;
	myInnerAngle = CommonUtilities::DegreeToRadian(anInnerAngle);
	myOuterAngle = CommonUtilities::DegreeToRadian(anOuterAngle);
	myConeIntensityDifference = aDifference;
	myPosition = aPosition;
	myLightDirection = aDirection.GetNormalized();
	myColor = aColor;
	myCastShadows = aCastShadows;

	if (myCastShadows && myShadowMap == nullptr)
	{
		CreateShadowMap();
	}
}

void SpotlightComponent::Update()
{
	if (!myIsActive)
	{
		return;
	}

	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<LitCmd_AddSpotlight>(*this));
}

void SpotlightComponent::SetRange(float aRange)
{
	myRange = aRange;
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
	myLightDirection = aDirection.GetNormalized();
}

void SpotlightComponent::SetColor(const CommonUtilities::Vector3f& aColor)
{
	myColor = aColor;
}

const SpotlightComponent* SpotlightComponent::GetTypePointer() const
{
	return this;
}

void SpotlightComponent::CreateShadowMap()
{
	myShadowMap = std::make_shared<Texture>();
	if (!RHI::CreateTexture(myShadowMap.get(), L"Spotlight_Shadow_Map", 512, 512, DXGI_FORMAT_R32_TYPELESS, D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE))
	{
		AMLogger.Err("SpotlightComponent: Failed to create shadowmap!");
		return;
	}
	RHI::ClearDepthStencil(myShadowMap.get());
}

float SpotlightComponent::GetRange() const
{
	return myRange;
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

void SpotlightComponent::SetCastShadows(bool aState)
{
	myCastShadows = aState;
	if (myCastShadows && myShadowMap == nullptr)
	{
		CreateShadowMap();
	}
}

void SpotlightComponent::ToogleCastShadows()
{
	SetCastShadows(!myCastShadows);
}

bool SpotlightComponent::IsCastingShadows() const
{
	return myCastShadows;
}

std::shared_ptr<Texture>& SpotlightComponent::GetShadowMap()
{
	return myShadowMap;
}

