#ifndef DEFAULTMATERIALDATA_HLSLI
#define DEFAULTMATERIALDATA_HLSLI
#include "../ConstantBuffers/ObjectBuffer.hlsli"
#include "../ConstantBuffers/FrameBuffer.hlsli"
#include "../ConstantBuffers/LightBuffer.hlsli"
#include "../ConstantBuffers/MaterialBuffer.hlsli"
#include "DefaultVertexInput.hlsli"
#include "ShaderFunctions.hlsli"

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
Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D DefaultTexture : register(t14);
Texture2D MissingTexture : register(t15);

#endif // DEFAULTMATERIALDATA_HLSLI