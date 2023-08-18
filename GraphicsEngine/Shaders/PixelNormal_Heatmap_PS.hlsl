#include "Default/DefaultMaterialData.hlsli"

DefaultPixelOutput main(DefaultVertexToPixel input)
{
    DefaultPixelOutput result;

    const float3x3 tbn = float3x3(input.TangentWS, input.BinormalWS, input.NormalWS);

    float3 pixelNormal = NormalTexture.Sample(DefaultSampler, input.UVs[0]);
    pixelNormal.xy = 2 * pixelNormal.xy - 1;
    pixelNormal.z = sqrt(1 - saturate(pixelNormal.x * pixelNormal.x + pixelNormal.y * pixelNormal.y));
    pixelNormal = normalize(mul(pixelNormal, tbn));

    result.Color.rgb = (pixelNormal + 1.f) * 0.5f;

    result.Color.w = 1;
    return result;
}