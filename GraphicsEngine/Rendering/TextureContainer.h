#pragma once
#include "Texture.h"
#include "Vector3.hpp"

struct TextureContainer
{
	Texture BackBuffer{};
	Texture DepthBuffer{};
	Texture BrdfLUTTexture{};

	Texture MissingTexture{};
	Texture DefaultNormalTexture{};
	Texture DefaultMaterialTexture{};
	Texture DefaultFXTexture{};
	Texture DefaultCubeMap{};

	void ClearTextures(const CommonUtilities::Vector3f& aFillColor);
};

struct SlotContainer
{
	unsigned BrdfLUTTextureSlot{};
	unsigned MissingTextureSlot{};
	unsigned DefaultNormalTextureSlot{};
	unsigned DefaultMaterialTextureSlot{};
	unsigned DefaultFXTextureSlot{};
	unsigned DefaultCubeMapSlot{};

	unsigned DirectionalShadowMapSlot{};
	unsigned PointShadowMapSlot{};
	unsigned SpotShadowMapSlot{};
};