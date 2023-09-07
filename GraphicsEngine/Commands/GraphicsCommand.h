#pragma once
#include "Rendering/Buffers/FrameBuffer.h"
#include "Rendering/Buffers/ObjectBuffer.h"
#include "Rendering/Buffers/LightBuffer.h"
#include "Rendering/Buffers/MaterialBuffer.h"

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

	Texture* GetMissingTexture();
	Texture* GetDefaultNormalTexture();
	Texture* GetDefaultMaterialTexture();
	Material* GetDefaultMaterial();

	CommonUtilities::Vector3f& GetWorldBoundsMin();
	CommonUtilities::Vector3f& GetWorldBoundsMax();
	CommonUtilities::Vector3f& GetWorldBoundsOrigin();
	float& GetWorldBoundsRadius();
};