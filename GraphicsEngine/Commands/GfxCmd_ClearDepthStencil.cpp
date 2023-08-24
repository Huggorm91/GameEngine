#include "GraphicsEngine.pch.h"
#include "GfxCmd_ClearDepthStencil.h"

GfxCmd_ClearDepthStencil::GfxCmd_ClearDepthStencil(const Texture* aDepthStencil): myDepthStencil(aDepthStencil)
{
}

void GfxCmd_ClearDepthStencil::Execute()
{
	RHI::ClearDepthStencil(myDepthStencil);
}
