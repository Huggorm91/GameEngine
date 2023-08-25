#pragma once
#include "LightCommand.h"
#include "AssetManager/Assets/Components/Light/SpotlightComponent.h"

class LitCmd_AddSpotlight : public LightCommand
{
public:
	LitCmd_AddSpotlight(SpotlightComponent& aLight);
	~LitCmd_AddSpotlight() override = default;

	void Execute(const int anIndex) override;
	void SetShadowMap(const int anIndex) override;

private:
	float myRange;
	float myIntensity;
	float myInnerAngle;
	float myOuterAngle;
	float myConeIntensityDifference;
	std::shared_ptr<Texture> myShadowMap;
	CommonUtilities::Vector3f myPosition;
	CommonUtilities::Vector3f myLightDirection;
	CommonUtilities::Vector3f myColor;
};