#ifndef MATERIALBUFFER_HLSLI
#define MATERIALBUFFER_HLSLI
cbuffer MaterialBuffer : register(b3)
{
    float4 MB_AlbedoColor;
    float4 MB_EmissionColor;
    float MB_EmissionIntensity;
    float2 MB_UVTiling;
    float MB_NormalStrength;
    float MB_Shininess;
    float MB_Metalness;
    
    float2 paddingmb;
}
#endif // MATERIALBUFFER_HLSLI