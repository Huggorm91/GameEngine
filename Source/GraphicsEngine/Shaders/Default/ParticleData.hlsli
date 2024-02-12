#ifndef DEFAULTPARTICLEDATA_HLSLI
#define DEFAULTPARTICLEDATA_HLSLI
#include "../ConstantBuffers/FrameBuffer.hlsli"
#include "../ConstantBuffers/ParticleBuffer.hlsli"
#include "Registers.hlsli"

struct SpriteVertexInput
{
    float4 Position : POSITION;
    float4 Color : COLOR;
    float3 Velocity : VELOCITY;
    float3 Direction : DIRECTION;
    float3 Scale : SCALE;
    float LifeTime : LIFETIME;
};

struct SpriteVertexToGeometry
{
    float4 WorldPosition : SV_POSITION;
    float4 Color : COLOR;
    float3 Velocity : VELOCITY;
    float3 Direction : DIRECTION;
    float3 Scale : SCALE;
    float LifeTime : LIFETIME;
};

struct SpriteGeometryToPixel
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float3 Velocity : VELOCITY;
    float3 Direction : DIRECTION;
    float2 UV : UV;
    float LifeTime : LIFETIME;
};

struct SpritePixelOutput
{
    float4 Color : SV_TARGET;
};

#endif // DEFAULTPARTICLEDATA_HLSLI