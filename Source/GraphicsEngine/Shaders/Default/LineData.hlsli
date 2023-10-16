#ifndef DEFAULTLINEDATA_HLSLI
#define DEFAULTLINEDATA_HLSLI
#include "../ConstantBuffers/FrameBuffer.hlsli"

struct DefaultVertexInput
{
    float4 Position : POSITION;
    float4 Color : COLOR;
    float Thickness : THICKNESS;
};

struct DefaultVertexToPixel
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

struct DefaultPixelOutput
{
    float4 Color : SV_TARGET;
};

#endif // DEFAULTLINEDATA_HLSLI