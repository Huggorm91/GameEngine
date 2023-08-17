#ifndef MATERIALBUFFER_HLSLI
#define MATERIALBUFFER_HLSLI
cbuffer MaterialBuffer : register(b3)
{
    float4 MB_AlbedoColor;
    float2 MB_UVTiling;
    float MB_NormalStrength;
    float MB_Shininess;
}
#endif // MATERIALBUFFER_HLSLI