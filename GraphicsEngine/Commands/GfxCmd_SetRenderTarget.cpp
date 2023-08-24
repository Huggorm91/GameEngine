#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetRenderTarget.h"

GfxCmd_SetRenderTarget::GfxCmd_SetRenderTarget(const Texture* aTarget, const Texture* aDepthStencil): myTarget(aTarget), myDepthStencil(aDepthStencil)
{
}

void GfxCmd_SetRenderTarget::Execute()
{
	RHI::SetRenderTarget(myTarget, myDepthStencil);
}
