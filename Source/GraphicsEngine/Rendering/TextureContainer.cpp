#include "GraphicsEngine.pch.h"
#include "TextureContainer.h"

void TextureContainer::ClearTextures(const Crimson::Vector3f& aFillColor)
{
	RHI::ClearRenderTarget(&BackBuffer, { aFillColor.x, aFillColor.y, aFillColor.z, 0.f });
	RHI::ClearRenderTarget(&Scenebuffer);
	RHI::ClearRenderTarget(&IntermediateA);
	RHI::ClearRenderTarget(&IntermediateB, { aFillColor.x, aFillColor.y, aFillColor.z, 0.f });
	RHI::ClearRenderTarget(&QuarterScenebufferA);
	RHI::ClearRenderTarget(&QuarterScenebufferB);
	RHI::ClearDepthStencil(&DepthBuffer);
}
