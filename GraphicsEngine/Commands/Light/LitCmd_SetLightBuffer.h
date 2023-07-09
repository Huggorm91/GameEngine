#pragma once
#include "LightCommand.h"

class LitCmd_SetLightBuffer : public LightCommand
{
	CommonUtilities::Vector3f myInvertedDirection;
	CommonUtilities::Vector3f myAmbientColor;
	CommonUtilities::Vector3f myDiffuseColor;
	CommonUtilities::Vector3f mySpecularColor;

public:
	LitCmd_SetLightBuffer(const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& anAmbientColor, const CommonUtilities::Vector3f& aDiffuseColor, const CommonUtilities::Vector3f& aSpecularColor);

	void Execute(const int anIndex) override;
};