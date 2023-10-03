#pragma once
#include "GraphicsCommand.h"

class GfxCmd_UpdateWorldBounds : public GraphicsCommand
{
public:
	GfxCmd_UpdateWorldBounds(const Crimson::Vector3f& aMin, const Crimson::Vector3f& aMax);
	~GfxCmd_UpdateWorldBounds() override = default;

	void Execute() override;

private:
	Crimson::Vector3f myMin;
	Crimson::Vector3f myMax;
};