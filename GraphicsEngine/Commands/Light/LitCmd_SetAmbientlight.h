#pragma once
#include "LightCommand.h"
#include "Rendering/Texture.h"

class LitCmd_SetAmbientlight : public LightCommand
{
public:
	// @param aCubemap If nullptr, will only adjust Intensity
	LitCmd_SetAmbientlight(const Texture* aCubemap, float anIntensity = 1.f);
	LitCmd_SetAmbientlight(const std::string& aCubemapPath, float anIntensity = 1.f);
	~LitCmd_SetAmbientlight() override = default;

	void Execute(const int anIndex) override;
	void SetShadowMap(const int anIndex) override;

private:
	const Texture* myCubemap;
	float myIntensity;
};
