#pragma once
#include "GraphicsCommand.h"

class GfxCmd_NewRenderlist : public GraphicsCommand
{
public:
	GfxCmd_NewRenderlist() = default;
	~GfxCmd_NewRenderlist() override = default;

	void Execute() override;
};