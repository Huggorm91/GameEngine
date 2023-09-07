#pragma once
#include "GraphicsCommand.h"
#include "../Rendering/Texture.h"

class GfxCmd_ClearDepthStencil : public GraphicsCommand
{
public:
	GfxCmd_ClearDepthStencil(const Texture* aDepthStencil, RenderStage aRenderStage = RenderStage::Deferred);
	~GfxCmd_ClearDepthStencil() override = default;

	void Execute() override;

private:
	const Texture* myDepthStencil;
};