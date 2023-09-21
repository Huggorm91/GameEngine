#pragma once
#include "../Component.h"
#include <Vector3.hpp>
#include "GraphicsEngine/Rendering/Texture.h"

class DirectionallightComponent : public Component
{
public:
	DirectionallightComponent();
	DirectionallightComponent(const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aColor = { 1.f, 1.f, 1.f }, float anIntensity = 1.f, bool aCastShadows = true);
	DirectionallightComponent(const Json::Value& aJson);
	DirectionallightComponent(const DirectionallightComponent& aLight);
	DirectionallightComponent(DirectionallightComponent&& aLight) = default;
	~DirectionallightComponent() = default;
	DirectionallightComponent& operator=(const DirectionallightComponent& aLight);
	DirectionallightComponent& operator=(DirectionallightComponent&& aLight) = default;

	void Init(const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aColor = { 1.f, 1.f, 1.f }, float anIntensity = 1.f, bool aCastShadows = true);
	void Update() override;

	void SetLightDirection(const CommonUtilities::Vector3f& aDirection);
	void SeColor(const CommonUtilities::Vector3f& aColor);
	void SetIntensity(float anIntensity);

	CommonUtilities::Vector3f GetLightDirection() const;
	const CommonUtilities::Vector3f& GetInvertedLightDirection() const;
	const CommonUtilities::Vector3f& GetColor() const;
	float GetIntensity() const;

	void SetCastShadows(bool aState);
	void ToogleCastShadows();
	bool IsCastingShadows() const;
	std::shared_ptr<Texture>& GetShadowMap();

	void CreateImGuiComponents(const std::string& aWindowName) override;
	Json::Value ToJson() const override;
	const DirectionallightComponent* GetTypePointer() const override;

private:
	CommonUtilities::Vector3f myInvertedLightDirection;
	CommonUtilities::Vector3f myLightDirection;
	CommonUtilities::Vector3f myColor;
	std::shared_ptr<Texture> myShadowMap;
	float myIntensity;
	bool myCastShadows;

	void CreateShadowMap();
};