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

Texture* GraphicsCommand::GetMissingTexture()
{
    return &GraphicsEngine::Get().myMissingTexture;
}

Texture* GraphicsCommand::GetDefaultNormalTexture()
{
    return &GraphicsEngine::Get().myDefaultNormalTexture;
}

Texture* GraphicsCommand::GetDefaultMaterialTexture()
{
    return &GraphicsEngine::Get().myDefaultMaterialTexture;
}

Material* GraphicsCommand::GetDefaultMaterial()
{
    return &GraphicsEngine::Get().myDefaultMaterial;
}

CommonUtilities::Vector3f& GraphicsCommand::GetWorldBoundsMin()
{
    return GraphicsEngine::Get().myWorldMin;
}

CommonUtilities::Vector3f& GraphicsCommand::GetWorldBoundsMax()
{
    return GraphicsEngine::Get().myWorldMax;
}

CommonUtilities::Vector3f& GraphicsCommand::GetWorldBoundsOrigin()
{
    return GraphicsEngine::Get().myWorldCenter;
}

float& GraphicsCommand::GetWorldBoundsRadius()
{
    return GraphicsEngine::Get().myWorldRadius;
}
