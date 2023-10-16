#pragma once
#include "LightCommand.h"

class LitCmd_SetShadowBias : public LightCommand
{
public:
	LitCmd_SetShadowBias(float aBias = 0.0005f);
	~LitCmd_SetShadowBias() override = default;

	void Execute(const int anIndex) override;

private:
	float myBias;
};

typedef LitCmd_SetShadowBias GfxCmd_SetShadowBias;