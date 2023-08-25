#pragma once
#include "GraphicsCommand.h"

class GfxCmd_UpdateWorldBounds : public GraphicsCommand
{
public:
	GfxCmd_UpdateWorldBounds(const CommonUtilities::Vector3f& aMin, const CommonUtilities::Vector3f& aMax);
	~GfxCmd_UpdateWorldBounds() override = default;

	void Execute() override;

private:
	CommonUtilities::Vector3f myMin;
	CommonUtilities::Vector3f myMax;
};