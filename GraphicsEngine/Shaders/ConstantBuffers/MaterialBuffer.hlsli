#ifndef MATERIALBUFFER_HLSLI
#define MATERIALBUFFER_HLSLI
cbuffer MaterialBuffer : register(b3)
{
    float4 MB_AlbedoColor;
    float2 MB_UVTiling;
    float MB_NormalStrength;
    float MB_Shininess;
    float MB_Metalness;
    
    float3 paddingmb;
}
#endif // MATERIALBUFFER_HLSLI