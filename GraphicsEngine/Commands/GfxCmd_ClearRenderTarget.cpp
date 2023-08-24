#include "GraphicsEngine.pch.h"
#include "GfxCmd_ClearRenderTarget.h"

GfxCmd_ClearRenderTarget::GfxCmd_ClearRenderTarget(const Texture* aTarget, const CommonUtilities::Vector4f& aColor): myTarget(aTarget), myColor(aColor)
{
}

void GfxCmd_ClearRenderTarget::Execute()
{
	RHI::ClearRenderTarget(myTarget, { myColor.x, myColor.y, myColor.z, myColor.w });
}
