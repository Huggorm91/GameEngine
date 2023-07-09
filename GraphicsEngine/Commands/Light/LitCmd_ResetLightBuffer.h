#pragma once
#include "LightCommand.h"

class LitCmd_ResetLightBuffer : public LightCommand
{
public:
	LitCmd_ResetLightBuffer() = default;

	void Execute(const int anIndex) override;
};