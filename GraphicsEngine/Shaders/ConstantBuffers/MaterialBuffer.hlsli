#ifndef MATERIALBUFFER_HLSLI
#define MATERIALBUFFER_HLSLI
cbuffer MaterialBuffer : register(b3)
{
    float4 AlbedoColor;
    float2 UVTiling;
    float NormalStrength;
    float Shininess;
}
#endif // MATERIALBUFFER_HLSLI