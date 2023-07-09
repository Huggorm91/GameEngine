#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetTextureResource.h"
#include "AssetManager/Managers/TextureManager.h"

GfxCmd_SetTextureResource::GfxCmd_SetTextureResource(Texture* anAlbedoTexture, Texture* aNormalTexture) : myAlbedoTexture(anAlbedoTexture), myNormalTexture(aNormalTexture)
{
}

void GfxCmd_SetTextureResource::Execute()
{
	if (myAlbedoTexture)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 0, myAlbedoTexture);
	}
	else
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 0, GetMissingTexture());
	}

	if (myNormalTexture)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 1, myNormalTexture);
	}
	else
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 1, GetDefaultNormalTexture());
	}
}
