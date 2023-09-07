#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetRenderTarget.h"

GfxCmd_SetRenderTarget::GfxCmd_SetRenderTarget(const Texture* aTarget, const Texture* aDepthStencil, RenderStage aRenderStage): GraphicsCommand(aRenderStage), myTarget(aTarget), myDepthStencil(aDepthStencil)
{
}

void GfxCmd_SetRenderTarget::Execute()
{
	RHI::SetRenderTarget(myTarget, myDepthStencil);
}
