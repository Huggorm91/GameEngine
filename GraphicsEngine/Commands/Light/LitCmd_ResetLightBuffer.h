#pragma once
#include "LightCommand.h"

class LitCmd_ResetLightBuffer : public LightCommand
{
public:
	LitCmd_ResetLightBuffer() = default;
	~LitCmd_ResetLightBuffer() override = default;

	void Execute(const int anIndex) override;
};

typedef LitCmd_ResetLightBuffer GfxCmd_ResetLightBuffer;