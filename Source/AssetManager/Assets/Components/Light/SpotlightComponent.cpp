#include "AssetManager.pch.h"
#include "SpotlightComponent.h"
#include "Assets/GameObject.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/Light/LitCmd_AddSpotlight.h"

SpotlightComponent::SpotlightComponent() :Component(ComponentType::Spotlight), myRange(), myIntensity(), myInnerAngle(), myOuterAngle(), myPosition(), myLightDirection(), myColor(), myCastShadows(false), myShadowMap(nullptr)
#ifndef _RETAIL
, myEditDirection(myLightDirection)
#endif // !_RETAIL
{
}

SpotlightComponent::SpotlightComponent(float aRange, float anIntensity, float anInnerAngle, float anOuterAngle, const Crimson::Vector3f& aDirection, const Crimson::Vector3f& aPosition, const Crimson::Vector3f& aColor, bool aCastShadows) :
	Component(ComponentType::Spotlight), myRange(aRange), myIntensity(anIntensity), myInnerAngle(Crimson::DegreeToRadian(anInnerAngle)), myOuterAngle(Crimson::DegreeToRadian(anOuterAngle)), myPosition(aPosition),
	myLightDirection(aDirection.GetNormalized()), myColor(aColor), myCastShadows(aCastShadows), myShadowMap(nullptr)
#ifndef _RETAIL
	, myEditDirection(myLightDirection)
#endif // !_RETAIL
{
	if (aCastShadows)
	{
		CreateShadowMap();
	}
}

SpotlightComponent::SpotlightComponent(const Json::Value& aJson) :Component(aJson), myRange(aJson["Range"].asFloat()), myIntensity(aJson["Intensity"].asFloat()), myInnerAngle(aJson["InnerAngle"].asFloat()), myOuterAngle(aJson["OuterAngle"].asFloat()),
myPosition(aJson["Position"]), myLightDirection(aJson["LightDirection"]), myColor(aJson["Color"]), myCastShadows(aJson["CastShadows"].asBool()), myShadowMap(nullptr)
#ifndef _RETAIL
, myEditDirection(myLightDirection)
#endif // !_RETAIL
{
	if (myCastShadows)
	{
		CreateShadowMap();
	}
}

SpotlightComponent::SpotlightComponent(const SpotlightComponent& aLight) :Component(aLight), myRange(aLight.myRange), myIntensity(aLight.myIntensity), myInnerAngle(aLight.myInnerAngle), myOuterAngle(aLight.myOuterAngle),
myPosition(aLight.myPosition), myLightDirection(aLight.myLightDirection), myColor(aLight.myColor), myCastShadows(aLight.myCastShadows), myShadowMap(nullptr)
#ifndef _RETAIL
, myEditDirection(aLight.myLightDirection)
#endif // !_RETAIL
{
	if (myCastShadows)
	{
		CreateShadowMap();
	}
}

SpotlightComponent& SpotlightComponent::operator=(const SpotlightComponent& aLight)
{
	Component::operator=(aLight);
	myRange = aLight.myRange;
	myIntensity = aLight.myIntensity;
	myInnerAngle = aLight.myInnerAngle;
	myOuterAngle = aLight.myOuterAngle;
	myPosition = aLight.myPosition;
	myLightDirection = aLight.myLightDirection;
	myColor = aLight.myColor;
	myCastShadows = aLight.myCastShadows;
#ifndef _RETAIL
	myEditDirection = aLight.myLightDirection;
#endif // !_RETAIL

	if (myCastShadows && myShadowMap == nullptr)
	{
		CreateShadowMap();
	}
	return *this;
}

void SpotlightComponent::Update()
{
	Render();
}

void SpotlightComponent::Render()
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

void SpotlightComponent::SetPosition(const Crimson::Vector3f& aPosition)
{
	myPosition = aPosition;
}

void SpotlightComponent::SetLightDirection(const Crimson::Vector3f& aDirection)
{
	myLightDirection = aDirection.GetNormalized();
#ifndef _RETAIL
	myEditDirection = myLightDirection;
#endif // !_RETAIL
}

void SpotlightComponent::SetColor(const Crimson::Vector3f& aColor)
{
	myColor = aColor;
}

void SpotlightComponent::CreateImGuiComponents(const std::string& aWindowName)
{
	Component::CreateImGuiComponents(aWindowName);
	ImGui::Checkbox("Cast Shadow", &myCastShadows);
	ImGui::DragFloat("Intensity", &myIntensity, 0.01f, 0.f, INFINITY, "%.3f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::ColorEdit3("Color", &myColor.x);
	ImGui::DragFloat("Range", &myRange);
	ImGui::DragFloat3("Position", &myPosition.x);
#ifndef _RETAIL
	if (ImGui::DragFloat3("Light Direction", &myEditDirection.x, .001f))
	{
		myLightDirection = myEditDirection.GetNormalizedNoAssert();
	}
#endif // _RETAIL
	ImGui::PushItemWidth(100.f);
	ImGui::SliderAngle("Inner Angle", &myInnerAngle, 0.f);
	ImGui::SameLine();
	ImGui::SliderAngle("Outer Angle", &myOuterAngle, 0.f);
	ImGui::PopItemWidth();
}

Json::Value SpotlightComponent::ToJson() const
{
	Json::Value result = Component::ToJson();
	result["Position"] = static_cast<Json::Value>(myPosition);
	result["LightDirection"] = static_cast<Json::Value>(myLightDirection);
	result["Color"] = myColor.ToJsonColor();
	result["Range"] = myRange;
	result["Intensity"] = myIntensity;
	result["InnerAngle"] = myInnerAngle;
	result["OuterAngle"] = myOuterAngle;
	result["CastShadows"] = myCastShadows;
	return result;
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

const Crimson::Vector3f& SpotlightComponent::GetPosition() const
{
	return myPosition;
}

const Crimson::Vector3f& SpotlightComponent::GetLightDirection() const
{
	return myLightDirection;
}

const Crimson::Vector3f& SpotlightComponent::GetColor() const
{
	return myColor;
}

Crimson::Matrix4x4f SpotlightComponent::GetTransform() const
{
	assert(myParent != nullptr && "PointlightComponent is not Initialized!");
	return myParent->GetTransformMatrix() * Crimson::Matrix4x4f::CreateTranslationMatrix(myPosition);
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

void SpotlightComponent::Serialize(std::ostream& aStream) const
{
	Component::Serialize(aStream);
	size_t size = sizeof(myPosition) + sizeof(myLightDirection) + sizeof(myColor) + sizeof(myRange) + sizeof(myIntensity) + sizeof(myInnerAngle) + sizeof(myOuterAngle) + sizeof(myCastShadows);
	aStream.write(reinterpret_cast<const char*>(&myPosition), size);
}

void SpotlightComponent::Deserialize(std::istream& aStream)
{
	Component::Deserialize(aStream);
	size_t size = sizeof(myPosition) + sizeof(myLightDirection) + sizeof(myColor) + sizeof(myRange) + sizeof(myIntensity) + sizeof(myInnerAngle) + sizeof(myOuterAngle) + sizeof(myCastShadows);
	aStream.read(reinterpret_cast<char*>(&myPosition), size);

#ifndef _RETAIL
	myEditDirection = myLightDirection;
#endif // !_RETAIL

	if (myCastShadows)
	{
		CreateShadowMap();
	}
}

