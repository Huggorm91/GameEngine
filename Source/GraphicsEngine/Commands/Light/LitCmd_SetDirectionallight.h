#pragma once
#include "LightCommand.h"
#include "AssetManager/Assets/Components/Light/DirectionallightComponent.h"

class LitCmd_SetDirectionallight : public LightCommand
{
public:
	LitCmd_SetDirectionallight(DirectionallightComponent& aLight);
	LitCmd_SetDirectionallight(const Crimson::Vector3f& aDirection, const Crimson::Vector3f& aColor, float anIntensity = 1.f);
	~LitCmd_SetDirectionallight() override = default;

	void Execute(const int anIndex) override;

	Texture* GetShadowMap() override;
	void SetShadowMap(const int anIndex) override;
	bool CastsShadow() const override;

private:
	Crimson::Vector3f myInvertedLightDirection;
	Crimson::Vector3f myColor;
	std::shared_ptr<Texture> myShadowMap;
	float myIntensity;
	bool myCastsShadow;
};

typedef LitCmd_SetDirectionallight GfxCmd_SetDirectionallight;