#include "GraphicsEngine.pch.h"
#include "LightCommand.h"
#include "GraphicsEngine.h"

LightCommand::LightCommand(): myType(Type::Other)
{
}

LightCommand::Type LightCommand::GetType()
{
    return myType;
}

LightCommand::LightCommand(Type aType) : myType(aType)
{
}

LightBuffer& LightCommand::GetLightBuffer()
{
    return GraphicsEngine::Get().myLightBuffer;
}

const Texture* LightCommand::GetDefaultCubemap()
{
    return &GraphicsEngine::Get().myDefaultCubeMap;
}

Texture*& LightCommand::GetDirectionalShadowMap()
{
    return GraphicsEngine::Get().myDirectionalShadowMap;
}

std::array<Texture*, MAX_LIGHTS>& LightCommand::GetPointlightShadowMap()
{
    return GraphicsEngine::Get().myPointShadowMap;
}

std::array<Texture*, MAX_LIGHTS>& LightCommand::GetSpotlightShadowMap()
{
    return GraphicsEngine::Get().mySpotShadowMap;
}
