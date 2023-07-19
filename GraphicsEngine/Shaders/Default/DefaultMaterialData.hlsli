#ifndef DEFAULTMATERIALDATA_HLSLI
#define DEFAULTMATERIALDATA_HLSLI
#include "../ConstantBuffers/ObjectBuffer.hlsli"
#include "../ConstantBuffers/FrameBuffer.hlsli"
#include "../ConstantBuffers/LightBuffer.hlsli"
#include "../ConstantBuffers/MaterialBuffer.hlsli"
#include "DefaultVertexInput.hlsli"
#include "ShaderFunctions.hlsli"
#include "LightFunctions.hlsli"

struct DefaultVertexToPixel
{
    float2 UVs[4] : UV;
    float4 Color : COLOR;

    float4 LocalPosition : LOCALPOSITION;
    float4 Position : SV_POSITION;

    float3 NormalWS : NORMAL_WS;
    float3 TangentWS : TANGENT_WS;
    float3 BinormalWS : BINORMAL_WS;

    float3 NormalOS : NORMAL_OS;
    float3 TangentOS : TANGENT_OS;
    float3 BinormalOS : BINORMAL_OS;
};

struct DefaultPixelOutput
{
	float4 Color : SV_TARGET;
};

SamplerState DefaultSampler : register(s0);
Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D DefaultTexture : register(t98);
Texture2D MissingTexture : register(t99);
TextureCube EnvironmentCubeMap : register(t100);

#endif // DEFAULTMATERIALDATA_HLSLI