#include "Default/DefaultLineData.hlsli"

DefaultPixelOutput main(DefaultVertexToPixel input)
{
    DefaultPixelOutput result;
    result.Color = input.Color;
    return result;
}