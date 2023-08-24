#pragma once
#include "GraphicsCommand.h"
#include "../Rendering/Texture.h"
#include "../InterOp/RHI.h"

class GfxCmd_SetTextureResource : public GraphicsCommand
{
public:
	enum class TextureSlot : unsigned
	{
		Albedo = 0,
		Normal = 1,
		Material = 2,
		EnvironmentCubeMap = 100
	};

	GfxCmd_SetTextureResource(const Texture* aTexture, TextureSlot aSlot, PIPELINE_STAGE aStage);

	void Execute() override;

private:
	const PIPELINE_STAGE myStage;
	const TextureSlot mySlot;
	const Texture* myTexture;
};