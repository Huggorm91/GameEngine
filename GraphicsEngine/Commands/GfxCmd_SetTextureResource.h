#pragma once
#include "GraphicsCommand.h"
#include "../Rendering/Texture.h"

class GfxCmd_SetTextureResource : public GraphicsCommand
{
public:
	GfxCmd_SetTextureResource(Texture* anAlbedoTexture, Texture* aNormalTexture);

	void Execute() override;

private:
	const Texture* myAlbedoTexture;
	const Texture* myNormalTexture;
};