#ifndef LIGHTBUFFER_HLSLI
#define LIGHTBUFFER_HLSLI

struct PointlightData
{
    float4x4 Transform;
    float3 Position;
    float Radius;
    float3 Color;
    float Intensity;
};

struct SpotlightData
{
    float4x4 Transform;
    float3 Position;
    float Radius;
    float3 Color;
    float Intensity;
    float3 LightDirection;
    float InnerAngle;
    float OuterAngle;
    float ConeIntensityDifference;

    float2 padding;
};

cbuffer LightBuffer : register(b2)
{
	float3 LB_InvertedDirection;
    float paddinglb0;
    float3 LB_DirectionallightColor;
    float paddinglb1;
	float3 LB_AmbientColor;
    float paddinglb2;
	float3 LB_DiffuseColor;
    float paddinglb3;
	float3 LB_SpecularColor;
    float paddinglb4;
    
    PointlightData LB_Pointlights[8];
    SpotlightData LB_Spotlights[8];
}
#endif // LIGHTBUFFER_HLSLI