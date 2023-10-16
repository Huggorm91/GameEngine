#include "../Default/LineData.hlsli"

DefaultVertexToPixel main(DefaultVertexInput input)
{
    DefaultVertexToPixel result;
    result.Position = mul(FB_View, input.Position);
    result.Position = mul(FB_Projection, result.Position);
    result.Color = input.Color;
    return result;
}