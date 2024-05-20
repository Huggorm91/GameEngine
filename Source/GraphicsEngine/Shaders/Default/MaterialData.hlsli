#ifndef DEFAULTMATERIALDATA_HLSLI
#define DEFAULTMATERIALDATA_HLSLI
#include "../ConstantBuffers/ObjectBuffer.hlsli"
#include "../ConstantBuffers/FrameBuffer.hlsli"
#include "../ConstantBuffers/LightBuffer.hlsli"
#include "../ConstantBuffers/MaterialBuffer.hlsli"
#include "VertexInput.hlsli"
#include "ShaderFunctions.hlsli"
#include "Registers.hlsli"

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
#endif // DEFAULTMATERIALDATA_HLSLI