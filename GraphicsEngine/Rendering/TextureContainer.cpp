#include "GraphicsEngine.pch.h"
#include "TextureContainer.h"

void TextureContainer::ClearTextures(const CommonUtilities::Vector3f& aFillColor)
{
	RHI::ClearRenderTarget(&BackBuffer, { aFillColor.x, aFillColor.y, aFillColor.z, 0.f });
	RHI::ClearDepthStencil(&DepthBuffer);
}
