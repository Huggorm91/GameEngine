#include "GraphicsEngine.pch.h"
#include "GraphicsCommand.h"
#include "GraphicsEngine.h"
#include "Rendering/Material.h"

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

Texture* GraphicsCommand::GetDefaultNormalTexture()
{
    return &GraphicsEngine::Get().myDefaultNormalTexture;
}

Texture* GraphicsCommand::GetMissingTexture()
{
    return &GraphicsEngine::Get().myMissingTexture;
}

Material* GraphicsCommand::GetDefaultMaterial()
{
    return &GraphicsEngine::Get().myDefaultMaterial;
}
