#ifndef DEFAULTMATERIALDATA_HLSLI
#define DEFAULTMATERIALDATA_HLSLI
#include "../ConstantBuffers/ObjectBuffer.hlsli"
#include "../ConstantBuffers/FrameBuffer.hlsli"
#include "../ConstantBuffers/LightBuffer.hlsli"
#include "../ConstantBuffers/MaterialBuffer.hlsli"
#include "DefaultVertexInput.hlsli"
#include "ShaderFunctions.hlsli"
static const float PI = 3.1415926535f;
static const float PI_INVERSE = 0.31830988618379067153776752674503f; // 1/PI

struct DefaultVertexToPixel
{
    float2 UVs[4] : UV;
    float4 Color[4] : COLOR;

    float4 LocalPosition : LOCALPOSITION;
    float4 WorldPosition : WORLDPOSITION;
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
SamplerState ShadowSampler : register(s14);
SamplerState LUTSampler : register(s15);

Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D MaterialTexture : register(t2);

Texture2D DirectionallightMap : register(t95);

Texture2D LUTTexture : register(t96);
Texture2D DefaultMaterialTexture : register(t97);
Texture2D DefaultNormalTexture : register(t98);
Texture2D MissingTexture : register(t99);

TextureCube EnvironmentCubeMap : register(t100);
#endif // DEFAULTMATERIALDATA_HLSLI