#include "../Default/LineData.hlsli"

DefaultVertexToPixel main(DefaultVertexInput input)
{
    DefaultVertexToPixel result;
    result.Position = input.Position;
    result.Color = input.Color;
    return result;
}