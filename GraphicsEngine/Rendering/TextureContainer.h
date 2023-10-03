#pragma once
#include "Texture.h"
#include "Math/Vector3.hpp"

struct TextureContainer
{
	Texture BackBuffer{};
	Texture DepthBuffer{};
	Texture BrdfLUTTexture{};

	Texture Scenebuffer{};
	Texture HalfScenebuffer{};
	Texture QuarterScenebufferA{};
	Texture QuarterScenebufferB{};

	Texture IntermediateA{};
	Texture IntermediateB{};

	Texture MissingTexture{};
	Texture DefaultNormalTexture{};
	Texture DefaultMaterialTexture{};
	Texture DefaultFXTexture{};
	Texture DefaultCubeMap{};

	void ClearTextures(const Crimson::Vector3f& aFillColor);
};

struct SlotContainer
{
	unsigned BrdfLUTTextureSlot{};
	unsigned GBufferSlot{};
	unsigned GBufferCount{};

	unsigned IntermediateASlot{};
	unsigned IntermediateBSlot{};

	unsigned MissingTextureSlot{};
	unsigned DefaultNormalTextureSlot{};
	unsigned DefaultMaterialTextureSlot{};
	unsigned DefaultFXTextureSlot{};
	unsigned DefaultCubeMapSlot{};

	unsigned DirectionalShadowMapSlot{};
	unsigned PointShadowMapSlot{};
	unsigned SpotShadowMapSlot{};
};