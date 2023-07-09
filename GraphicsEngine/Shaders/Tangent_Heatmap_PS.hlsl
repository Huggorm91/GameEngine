#include "Default/DefaultMaterialData.hlsli"

DefaultPixelOutput main(DefaultVertexToPixel input)
{
    DefaultPixelOutput result;
    result.Color.rgb = (input.TangentOS + 1.f) * 0.5f;
    result.Color.w = 1;
    return result;
}