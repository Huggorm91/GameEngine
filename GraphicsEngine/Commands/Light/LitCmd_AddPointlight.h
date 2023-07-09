#pragma once
#include "LightCommand.h"
#include "AssetManager/Assets/Components/Light/PointlightComponent.h"

class LitCmd_AddPointlight : public LightCommand
{
public:
	LitCmd_AddPointlight(const PointlightComponent& aLight);

	void Execute(const int anIndex) override;

private:
	float myRadius;
	float myIntensity;
	CommonUtilities::Vector3f myPosition;
	CommonUtilities::Vector3f myColor;
};