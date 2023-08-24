#pragma once
#include "GraphicsCommand.h"
#include "../Rendering/Texture.h"

class GfxCmd_SetRenderTarget : public GraphicsCommand
{
public:
	GfxCmd_SetRenderTarget(const Texture* aTarget, const Texture* aDepthStencil);

	void Execute() override;

private:
	const Texture* myTarget;
	const Texture* myDepthStencil;
};