#pragma once
#include "../Component.h"
#include <Matrix4x4.hpp>
#include "GraphicsEngine/Rendering/Texture.h"

class SpotlightComponent : public Component
{
public:
	SpotlightComponent();
	SpotlightComponent(float aRange, float anIntensity, float anInnerAngle, float anOuterAngle, float aDifference, const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aPosition = CommonUtilities::Vector3f::Null, const CommonUtilities::Vector3f& aColor = { 1.f, 1.f, 1.f }, bool aCastShadows = true);
	SpotlightComponent(const SpotlightComponent& aLight);
	~SpotlightComponent() = default;
	SpotlightComponent& operator=(const SpotlightComponent& aLight);
	//SpotlightComponent(const Json::Value& aJson, GameObject* aParent);

	void Init(float aRange, float anIntensity, float anInnerAngle, float anOuterAngle, float aDifference, const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aPosition = CommonUtilities::Vector3f::Null, const CommonUtilities::Vector3f& aColor = { 1.f, 1.f, 1.f }, bool aCastShadows = true);
	void Update() override;

	void SetRange(float aRange);
	void SetIntensity(float anIntensity);
	void SetInnerAngle(float anAngle);
	void SetOuterAngle(float anAngle);
	void SetConeIntensityDifference(float aDifference);
	void SetPosition(const CommonUtilities::Vector3f& aPosition);
	void SetLightDirection(const CommonUtilities::Vector3f& aDirection);
	void SetColor(const CommonUtilities::Vector3f& aColor);

	float GetRange() const;
	float GetIntensity() const;
	float GetInnerAngle() const;
	float GetOuterAngle() const;
	float GetConeIntensityDifference() const;
	const CommonUtilities::Vector3f& GetPosition() const;
	const CommonUtilities::Vector3f& GetLightDirection() const;
	const CommonUtilities::Vector3f& GetColor() const;
	CommonUtilities::Matrix4x4f GetTransform() const;

	void SetCastShadows(bool aState);
	void ToogleCastShadows();
	bool IsCastingShadows() const;
	std::shared_ptr<Texture>& GetShadowMap();

	//Json::Value ToJson() const override;
	const SpotlightComponent* GetTypePointer() const override;

private:
	CommonUtilities::Vector3f myPosition;
	CommonUtilities::Vector3f myLightDirection;
	CommonUtilities::Vector3f myColor;
	std::shared_ptr<Texture> myShadowMap;
	float myRange;
	float myIntensity;
	float myInnerAngle;
	float myOuterAngle;
	float myConeIntensityDifference;  // Potentially remove
	bool myCastShadows;

	void CreateShadowMap();
};