#include "AssetManager.pch.h"
#include "PointlightComponent.h"
#include "Assets/GameObject.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/Light/LitCmd_AddPointlight.h"

PointlightComponent::PointlightComponent():Component(ComponentType::Pointlight), myRadius(), myIntensity(), myPosition(), myColor(1.f, 1.f, 1.f), myCastShadows(false), myShadowMap(nullptr)
{
}

PointlightComponent::PointlightComponent(float aRadius, float anIntensity, const Crimson::Vector3f& aColor, const Crimson::Vector3f& aPosition, bool aCastShadows) :Component(ComponentType::Pointlight), myRadius(aRadius), myIntensity(anIntensity), myPosition(aPosition), myColor(aColor), myCastShadows(aCastShadows), myShadowMap(nullptr)
{
	if (aCastShadows)
	{
		CreateShadowMap();
	}
}

PointlightComponent::PointlightComponent(const Json::Value& aJson) :Component(aJson), myRadius(aJson["Radius"].asFloat()), myIntensity(aJson["Intensity"].asFloat()), myPosition(aJson["Position"]), myColor(aJson["Color"]), 
myCastShadows(aJson["CastShadows"].asBool()), myShadowMap(nullptr)
{
	if (myCastShadows)
	{
		CreateShadowMap();
	}
}

PointlightComponent::PointlightComponent(const PointlightComponent& aLight) :Component(aLight), myRadius(aLight.myRadius), myIntensity(aLight.myIntensity), myPosition(aLight.myPosition), myColor(aLight.myColor), myCastShadows(aLight.myCastShadows), myShadowMap(nullptr)
{
	if (myCastShadows)
	{
		CreateShadowMap();
	}
}

PointlightComponent& PointlightComponent::operator=(const PointlightComponent& aLight)
{
	Component::operator=(aLight);
	myRadius = aLight.myRadius;
	myIntensity = aLight.myIntensity;
	myPosition = aLight.myPosition;
	myColor = aLight.myColor;
	myCastShadows = aLight.myCastShadows;

	if (myCastShadows && myShadowMap == nullptr)
	{
		CreateShadowMap();
	}
	return *this;
}

void PointlightComponent::Init(float aRadius, float anIntensity, const Crimson::Vector3f& aColor, const Crimson::Vector3f& aPosition, bool aCastShadows)
{
	myRadius = aRadius;
	myIntensity = anIntensity;
	myPosition = aPosition;
	myColor = aColor;
	myCastShadows = aCastShadows;

	if (myCastShadows && myShadowMap == nullptr)
	{
		CreateShadowMap();
	}
}

void PointlightComponent::Update()
{
	Render();
}

void PointlightComponent::Render()
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

void PointlightComponent::SetPosition(const Crimson::Vector3f& aPosition)
{
	myPosition = aPosition;
}

void PointlightComponent::SetColor(const Crimson::Vector3f& aColor)
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

const Crimson::Vector3f& PointlightComponent::GetPosition() const
{
	return myPosition;
}

const Crimson::Vector3f& PointlightComponent::GetColor() const
{
	return myColor;
}

Crimson::Matrix4x4f PointlightComponent::GetTransform() const
{
	assert(myParent != nullptr && "PointlightComponent is not Initialized!");
	return myParent->GetTransformMatrix() * Crimson::Matrix4x4f::CreateTranslationMatrix(myPosition);
}

void PointlightComponent::SetCastShadows(bool aState)
{
	myCastShadows = aState;
	if (myCastShadows && myShadowMap == nullptr)
	{
		CreateShadowMap();
	}
}

void PointlightComponent::ToogleCastShadows()
{
	SetCastShadows(!myCastShadows);
}

bool PointlightComponent::IsCastingShadows() const
{
	return myCastShadows;
}

std::shared_ptr<Texture>& PointlightComponent::GetShadowMap()
{
	return myShadowMap;
}

void PointlightComponent::Serialize(std::ostream& aStream) const
{
	Component::Serialize(aStream);
	size_t size = sizeof(myPosition) + sizeof(myColor) + sizeof(myRadius) + sizeof(myIntensity) + sizeof(myCastShadows);
	aStream.write(reinterpret_cast<const char*>(&myPosition), size);
}

void PointlightComponent::Deserialize(std::istream& aStream)
{
	Component::Deserialize(aStream);
	size_t size = sizeof(myPosition) + sizeof(myColor) + sizeof(myRadius) + sizeof(myIntensity) + sizeof(myCastShadows);
	aStream.read(reinterpret_cast<char*>(&myPosition), size);

	if (myCastShadows)
	{
		CreateShadowMap();
	}
}

void PointlightComponent::CreateImGuiComponents(const std::string& aWindowName)
{
	Component::CreateImGuiComponents(aWindowName);
	ImGui::Checkbox("Cast Shadow", &myCastShadows);
	ImGui::DragFloat("Intensity", &myIntensity, 0.01f, 0.f, INFINITY, "%.3f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::ColorEdit3("Color", &myColor.x);
	ImGui::DragFloat("Radius", &myRadius);
	ImGui::DragFloat3("Position", &myPosition.x);
}

Json::Value PointlightComponent::ToJson() const
{
	Json::Value result = Component::ToJson();
	result["Position"] = static_cast<Json::Value>(myPosition);
	result["Color"] = myColor.ToJsonColor();
	result["Radius"] = myRadius;
	result["Intensity"] = myIntensity;
	result["CastShadows"] = myCastShadows;
	return result;
}

inline std::string PointlightComponent::ToString() const
{
	return "Pointlight";
}

const PointlightComponent* PointlightComponent::GetTypePointer() const
{
	return this;
}

void PointlightComponent::CreateShadowMap()
{
	myShadowMap = std::make_shared<Texture>();
	if (!RHI::CreateTextureCube(myShadowMap.get(), L"Pointlight_Shadow_Map", 512, 512, DXGI_FORMAT_R32_TYPELESS, D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE))
	{
		AMLogger.Err("PointlightComponent: Failed to create shadowmap!");
		return;
	}
	RHI::ClearDepthStencil(myShadowMap.get());
}
