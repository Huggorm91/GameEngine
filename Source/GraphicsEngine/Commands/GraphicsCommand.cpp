#include "GraphicsEngine.pch.h"
#include "GraphicsCommand.h"
#include "GraphicsEngine.h"
#include "Rendering/Material.h"

GraphicsCommand::GraphicsCommand(RenderStage aStage): myStage(aStage)
{
}

GraphicsCommand::RenderStage GraphicsCommand::GetRenderStage() const
{
    return myStage;
}

FrameBuffer& GraphicsCommand::GetFrameBuffer()
{
    return GraphicsEngine::Get().myFrameBuffer;
}

ObjectBuffer& GraphicsCommand::GetObjectBuffer()
{
    return GraphicsEngine::Get().myObjectBuffer;
}

LightBuffer& GraphicsCommand::GetLightBuffer()
{
    return GraphicsEngine::Get().myLightBuffer;
}

MaterialBuffer& GraphicsCommand::GetMaterialBuffer()
{
    return GraphicsEngine::Get().myMaterialBuffer;
}

unsigned GraphicsCommand::GetFrameBufferSlot()
{
    return GraphicsEngine::Get().myFrameBufferSlot;
}

unsigned GraphicsCommand::GetObjectBufferSlot()
{
    return GraphicsEngine::Get().myObjectBufferSlot;
}

unsigned GraphicsCommand::GetLightBufferSlot()
{
    return GraphicsEngine::Get().myLightBufferSlot;
}

unsigned GraphicsCommand::GetMaterialBufferSlot()
{
    return GraphicsEngine::Get().myMaterialBufferSlot;
}

Texture* GraphicsCommand::GetMissingTexture()
{
    return &GraphicsEngine::Get().myTextures.MissingTexture;
}

Texture* GraphicsCommand::GetDefaultNormalTexture()
{
    return &GraphicsEngine::Get().myTextures.DefaultNormalTexture;
}

Texture* GraphicsCommand::GetDefaultMaterialTexture()
{
    return &GraphicsEngine::Get().myTextures.DefaultMaterialTexture;
}

Texture* GraphicsCommand::GetDefaultFXTexture()
{
    return &GraphicsEngine::Get().myTextures.DefaultFXTexture;
}

Material* GraphicsCommand::GetDefaultMaterial()
{
    return &GraphicsEngine::Get().myDefaultMaterial;
}

const SlotContainer& GraphicsCommand::GetTextureSlots()
{
    return GraphicsEngine::Get().myTextureSlots;
}

Crimson::Vector3f& GraphicsCommand::GetWorldBoundsMin()
{
    return GraphicsEngine::Get().myWorldMin;
}

Crimson::Vector3f& GraphicsCommand::GetWorldBoundsMax()
{
    return GraphicsEngine::Get().myWorldMax;
}

Crimson::Vector3f& GraphicsCommand::GetWorldBoundsOrigin()
{
    return GraphicsEngine::Get().myWorldCenter;
}

float& GraphicsCommand::GetWorldBoundsRadius()
{
    return GraphicsEngine::Get().myWorldRadius;
}
