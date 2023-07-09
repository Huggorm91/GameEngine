#pragma once
#include "LightCommand.h"
#include "AssetManager/Assets/Components/Light/DirectionallightComponent.h"

class LitCmd_SetDirectionallight : public LightCommand
{
public:
	LitCmd_SetDirectionallight(const DirectionallightComponent& aLight);
	LitCmd_SetDirectionallight(const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aColor);

	void Execute(const int anIndex) override;

private:
	CommonUtilities::Vector3f myInvertedLightDirection;
	CommonUtilities::Vector3f myColor;
};

