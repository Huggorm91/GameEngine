#ifndef GBUFFERDATA_HLSLI
#define GBUFFERDATA_HLSLI
#include "DefaultMaterialData.hlsli"

struct GBufferOutput
{
    float4 Albedo : SV_TARGET0;
    float4 Normal : SV_TARGET1;
    float4 Material : SV_TARGET2;
    float4 VertexNormal : SV_TARGET3;
    float4 WorldPosition : SV_TARGET4;
    float4 ObjectID : SV_TARGET5;
};

Texture2D GBuffer_Albedo : register(t3);
Texture2D GBuffer_Material : register(t4);
Texture2D GBuffer_VertexNormal : register(t5);
Texture2D GBuffer_PixelNormal : register(t6);
Texture2D GBuffer_Position : register(t7);
Texture2D GBuffer_ObjectID : register(t8);

#endif // GBUFFERDATA_HLSLI