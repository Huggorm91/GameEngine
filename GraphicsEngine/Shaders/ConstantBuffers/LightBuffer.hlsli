#ifndef LIGHTBUFFER_HLSLI
#define LIGHTBUFFER_HLSLI

struct PointlightData
{
    float4x4 View;
    float4x4 Projection;
    float3 Position;
    float Radius;
    float3 Color;
    float Intensity;
};

struct SpotlightData
{
    float4x4 View;
    float4x4 Projection;
    float3 Position;
    float Range;
    float3 Color;
    float Intensity;
    float3 LightDirection;
    float InnerAngle;
    float OuterAngle;

    float3 padding;
};

cbuffer LightBuffer : register(b2)
{
    float4x4 LB_DirectionalView;
    float4x4 LB_DirectionalProjection;
	float3 LB_InvertedDirection;
    float LB_DirectionallightIntensity;
    float3 LB_DirectionallightColor;
    float LB_AmbientlightIntensity;
    
    PointlightData LB_Pointlights[8];
    SpotlightData LB_Spotlights[8];
}
#endif // LIGHTBUFFER_HLSLI