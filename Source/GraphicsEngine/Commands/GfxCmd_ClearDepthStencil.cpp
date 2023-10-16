#include "GraphicsEngine.pch.h"
#include "GfxCmd_ClearDepthStencil.h"

GfxCmd_ClearDepthStencil::GfxCmd_ClearDepthStencil(const Texture* aDepthStencil, RenderStage aRenderStage): GraphicsCommand(aRenderStage), myDepthStencil(aDepthStencil)
{
}

void GfxCmd_ClearDepthStencil::Execute()
{
	RHI::ClearDepthStencil(myDepthStencil);
}
