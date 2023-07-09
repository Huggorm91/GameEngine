#ifndef LIGHTBUFFER_HLSLI
#define LIGHTBUFFER_HLSLI

struct PointlightData
{
    float4x4 myTransform;
    float3 myPosition;
    float myRadius;
    float3 myColor;
    float myIntensity;
};

struct SpotlightData
{
    float4x4 myTransform;
    float3 myPosition;
    float myRadius;
    float3 myColor;
    float myIntensity;
    float3 myLightDirection;
    float myInnerAngle;
    float myOuterAngle;
    float myConeIntensityDifference;

    float2 padding;
};

cbuffer LightBuffer : register(b2)
{
	float3 LB_InvertedDirection;
    float paddinglb0;
	float3 LB_AmbientColor;
    float paddinglb1;
	float3 LB_DiffuseColor;
    float paddinglb2;
	float3 LB_SpecularColor;
    float paddinglb3;
    
    PointlightData LB_Pointlights[8];
    SpotlightData LB_Spotlights[8];
}
#endif // LIGHTBUFFER_HLSLI