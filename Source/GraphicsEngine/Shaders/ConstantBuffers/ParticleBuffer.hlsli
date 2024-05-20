#ifndef PARTICLEBUFFER_HLSLI
#define PARTICLEBUFFER_HLSLI
cbuffer FrameBuffer : register(b4)
{
    float PB_MaxLifeTime;
    float3 PB_StartSize;
    float paddingpb;
    float3 PB_EndSize;
    float4 PB_StartColor;
    float4 PB_EndColor;
    float4x4 PB_Transform;
}
#endif // PARTICLEBUFFER_HLSLI