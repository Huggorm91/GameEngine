#include "AssetManager.pch.h"
#include "DirectionallightComponent.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetDirectionallight.h"

DirectionallightComponent::DirectionallightComponent() : Component(ComponentType::Directionallight), myInvertedLightDirection(), myColor(1.f, 1.f, 1.f), myIntensity(1.f), myCastShadows(false), myShadowMap(nullptr), myLightDirection()
{
}

DirectionallightComponent::DirectionallightComponent(const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aColor, float anIntensity, bool aCastShadows) :Component(ComponentType::Directionallight), myInvertedLightDirection(-aDirection.GetNormalized()), myColor(aColor), myIntensity(anIntensity), myCastShadows(aCastShadows), myShadowMap(nullptr), myLightDirection(-myInvertedLightDirection)
{
	if (aCastShadows)
	{
		CreateShadowMap();
	}
}

DirectionallightComponent::DirectionallightComponent(const Json::Value& aJson) :Component(aJson), myIntensity(aJson["Intensity"].asFloat()), myInvertedLightDirection(aJson["InvertedLightDirection"]), myColor(aJson["Color"]), 
myCastShadows(aJson["CastShadows"].asBool()), myShadowMap(nullptr), myLightDirection(-myInvertedLightDirection)
{
	if (myCastShadows)
	{
		CreateShadowMap();
	}
}

DirectionallightComponent::DirectionallightComponent(const DirectionallightComponent& aLight) :Component(aLight), myInvertedLightDirection(aLight.myInvertedLightDirection), myColor(aLight.myColor), myIntensity(aLight.myIntensity), myCastShadows(aLight.myCastShadows), myShadowMap(nullptr), myLightDirection(aLight.myLightDirection)
{
	if (myCastShadows)
	{
		CreateShadowMap();
	}
}

DirectionallightComponent& DirectionallightComponent::operator=(const DirectionallightComponent& aLight)
{
	Component::operator=(aLight);
	myInvertedLightDirection = aLight.myInvertedLightDirection;
	myLightDirection = aLight.myLightDirection;
	myColor = aLight.myColor;
	myIntensity = aLight.myIntensity;
	myCastShadows = aLight.myCastShadows;

	if (myCastShadows && myShadowMap == nullptr)
	{
		CreateShadowMap();
	}
	return *this;
}

void DirectionallightComponent::Init(const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aColor, float anIntensity, bool aCastShadows)
{
	myLightDirection = aDirection.GetNormalized();
	myInvertedLightDirection = -myLightDirection;
	myColor = aColor;
	myIntensity = anIntensity;
	myCastShadows = aCastShadows;

	if (myCastShadows && myShadowMap == nullptr)
	{
		CreateShadowMap();
	}
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
	myLightDirection = aDirection.GetNormalized();
	myInvertedLightDirection = -myLightDirection;
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
#ifndef _RETAIL
	return -myInvertedLightDirection;
#else
	return myLightDirection;
#endif // !_RETAIL	
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

void DirectionallightComponent::SetCastShadows(bool aState)
{
	myCastShadows = aState;
	if (myCastShadows && myShadowMap.get() == nullptr)
	{
		CreateShadowMap();
	}
}

void DirectionallightComponent::ToogleCastShadows()
{
	SetCastShadows(!myCastShadows);
}

bool DirectionallightComponent::IsCastingShadows() const
{
	return myCastShadows;
}

std::shared_ptr<Texture>& DirectionallightComponent::GetShadowMap()
{
	return myShadowMap;
}

void DirectionallightComponent::CreateImGuiComponents(const std::string& aWindowName)
{
	Component::CreateImGuiComponents(aWindowName);
	ImGui::Checkbox("Cast Shadow", &myCastShadows);
	ImGui::DragFloat("Intensity", &myIntensity, 0.01f, 0.f, INFINITY, "%.3f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::ColorEdit3("Color", &myColor.x);
	if (ImGui::DragFloat3("Light Direction", &myLightDirection.x))
	{
		myInvertedLightDirection = -myLightDirection.GetNormalized();
	}
}

Json::Value DirectionallightComponent::ToJson() const
{
	Json::Value result = Component::ToJson();
	result["InvertedLightDirection"] = static_cast<Json::Value>(myInvertedLightDirection);
	result["Color"] = static_cast<Json::Value>(myColor);
	result["Intensity"] = myIntensity;
	result["CastShadows"] = myCastShadows;
	return result;
}

const DirectionallightComponent* DirectionallightComponent::GetTypePointer() const
{
	return this;
}

void DirectionallightComponent::CreateShadowMap()
{
	myShadowMap = std::make_shared<Texture>();
	if (!RHI::CreateTexture(myShadowMap.get(), L"Directionallight_Shadow_Map", 1024, 1024, DXGI_FORMAT_R32_TYPELESS, D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE))
	{
		AMLogger.Err("DirectionallightComponent: Failed to create shadowmap!");
		return;
	}
	RHI::ClearDepthStencil(myShadowMap.get());
}
