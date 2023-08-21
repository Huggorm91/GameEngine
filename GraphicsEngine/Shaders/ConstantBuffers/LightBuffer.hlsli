#ifndef LIGHTBUFFER_HLSLI
#define LIGHTBUFFER_HLSLI

struct PointlightData
{
    float3 Position;
    float Radius;
    float3 Color;
    float Intensity;
};

struct SpotlightData
{
    float3 Position;
    float Range;
    float3 Color;
    float Intensity;
    float3 LightDirection;
    float InnerAngle;
    float OuterAngle;
    float ConeIntensityDifference; // Potentially remove

    float2 padding;
};

cbuffer LightBuffer : register(b2)
{
	float3 LB_InvertedDirection;
    float LB_DirectionallightIntensity;
    float3 LB_DirectionallightColor;
    float LB_AmbientlightIntensity;
    
    PointlightData LB_Pointlights[8];
    SpotlightData LB_Spotlights[8];
}
#endif // LIGHTBUFFER_HLSLI