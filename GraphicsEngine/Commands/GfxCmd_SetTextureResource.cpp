#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetTextureResource.h"
#include "AssetManager/Managers/TextureManager.h"

GfxCmd_SetTextureResource::GfxCmd_SetTextureResource(const Texture* aTexture, TextureSlot aSlot, PIPELINE_STAGE aStage) : myTexture(aTexture), mySlot(aSlot), myStage(aStage)
{
}

void GfxCmd_SetTextureResource::Execute()
{
		RHI::SetTextureResource(myStage, static_cast<unsigned>(mySlot), myTexture);
}
