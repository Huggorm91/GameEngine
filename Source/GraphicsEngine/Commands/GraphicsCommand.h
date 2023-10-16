#pragma once
#include "Rendering/Buffers/FrameBuffer.h"
#include "Rendering/Buffers/ObjectBuffer.h"
#include "Rendering/Buffers/LightBuffer.h"
#include "Rendering/Buffers/MaterialBuffer.h"
#include "Rendering/TextureContainer.h"

class Material;

class GraphicsCommand
{
public:
	enum class RenderStage
	{
		Light,
		Shadow,
		Deferred,
		Forward,
		PostProcess
	};

	GraphicsCommand(RenderStage aStage = RenderStage::Deferred);
	virtual ~GraphicsCommand() = default;
	virtual void Execute() = 0;
	RenderStage GetRenderStage() const;

protected:
	RenderStage myStage;

	FrameBuffer& GetFrameBuffer();
	ObjectBuffer& GetObjectBuffer();
	LightBuffer& GetLightBuffer();
	MaterialBuffer& GetMaterialBuffer();

	unsigned GetFrameBufferSlot();
	unsigned GetObjectBufferSlot();
	unsigned GetLightBufferSlot();
	unsigned GetMaterialBufferSlot();

	Texture* GetMissingTexture();
	Texture* GetDefaultNormalTexture();
	Texture* GetDefaultMaterialTexture();
	Texture* GetDefaultFXTexture();
	Material* GetDefaultMaterial();
	const SlotContainer& GetTextureSlots();

	Crimson::Vector3f& GetWorldBoundsMin();
	Crimson::Vector3f& GetWorldBoundsMax();
	Crimson::Vector3f& GetWorldBoundsOrigin();
	float& GetWorldBoundsRadius();
};