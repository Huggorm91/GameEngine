#ifndef QUADDATA_HLSLI
#define QUADDATA_HLSLI

struct QuadVSToPS
{
    float2 UV : UV;
    float4 Position : SV_POSITION;
};

#endif // QUADDATA_HLSLI