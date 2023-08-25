#pragma once
#include "Rendering/Buffers/FrameBuffer.h"
#include "Rendering/Buffers/ObjectBuffer.h"
#include "Rendering/Buffers/LightBuffer.h"
#include "Rendering/Buffers/MaterialBuffer.h"

class Material;

class GraphicsCommand
{
public:
	virtual ~GraphicsCommand() = default;
	virtual void Execute() = 0;

protected:
	FrameBuffer& GetFrameBuffer();
	ObjectBuffer& GetObjectBuffer();
	LightBuffer& GetLightBuffer();
	MaterialBuffer& GetMaterialBuffer();

	Texture* GetMissingTexture();
	Texture* GetDefaultNormalTexture();
	Texture* GetDefaultMaterialTexture();
	Material* GetDefaultMaterial();

	CommonUtilities::Vector3f& GetWorldBoundsMin();
	CommonUtilities::Vector3f& GetWorldBoundsMax();
};