#pragma once
#include "LightCommand.h"
#include "AssetManager/Assets/Components/Light/SpotlightComponent.h"

class LitCmd_AddSpotlight : public LightCommand
{
public:
	LitCmd_AddSpotlight(SpotlightComponent& aLight);
	~LitCmd_AddSpotlight() override = default;

	void Execute(const int anIndex) override;

	Texture* GetShadowMap() override;
	void SetShadowMap(const int anIndex) override;
	bool CastsShadow() const override;

private:
	bool myCastsShadow;
	float myRange;
	float myIntensity;
	float myInnerAngle;
	float myOuterAngle;
	std::shared_ptr<Texture> myShadowMap;
	Crimson::Vector3f myPosition;
	Crimson::Vector3f myLightDirection;
	Crimson::Vector3f myColor;
};

typedef LitCmd_AddSpotlight GfxCmd_AddSpotlight;