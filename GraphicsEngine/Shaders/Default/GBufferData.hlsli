#ifndef GBUFFERDATA_HLSLI
#define GBUFFERDATA_HLSLI
#include "QuadData.hlsli"
#include "MaterialData.hlsli"

struct GBufferOutput
{
    float4 Albedo : SV_TARGET0;
    float4 Material : SV_TARGET1;
    float4 VertexNormal : SV_TARGET2;
    float4 Normal : SV_TARGET3;
    float4 WorldPosition : SV_TARGET4;
    float4 FX : SV_TARGET5;
    float4 ObjectID : SV_TARGET6;
};

Texture2D GBuffer_Albedo : register(t10);
Texture2D GBuffer_Material : register(t11);
Texture2D GBuffer_VertexNormal : register(t12);
Texture2D GBuffer_PixelNormal : register(t13);
Texture2D GBuffer_Position : register(t14);
Texture2D GBuffer_FX : register(t15);
Texture2D GBuffer_ObjectID : register(t16);

#endif // GBUFFERDATA_HLSLI