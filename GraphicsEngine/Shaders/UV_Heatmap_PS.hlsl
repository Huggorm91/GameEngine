#include "Default/DefaultMaterialData.hlsli"

DefaultPixelOutput main(DefaultVertexToPixel input)
{
    DefaultPixelOutput result;
    result.Color.x = input.UVs[0].x;
    result.Color.y = input.UVs[0].y;
    result.Color.w = 1;
    return result;
}