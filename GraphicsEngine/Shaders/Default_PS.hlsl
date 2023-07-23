#include "Default/DefaultMaterialData.hlsli"

DefaultPixelOutput main(DefaultVertexToPixel input)
{
	DefaultPixelOutput result;

	float4 textureColor = AlbedoTexture.Sample(DefaultSampler, input.UVs[0]);
	result.Color = GetAlphaBlendColor(input.Color[0], textureColor);
    //result.Color = GetAdditiveBlendColor(input.Color[0], textureColor);

    float3 pixelNormal = NormalTexture.Sample(DefaultSampler, input.UVs[0]);
    pixelNormal = 2 * pixelNormal - 1;
    pixelNormal.z = sqrt(1 - saturate(pixelNormal.x * pixelNormal.x + pixelNormal.y * pixelNormal.y));
    pixelNormal = normalize(mul(pixelNormal, float3x3(input.TangentWS, input.BinormalWS, input.NormalWS)));

    result.Color.rgb = CalculateBlinnPhong(pixelNormal, input.Position.xyz, result.Color.rgb, 62, FB_CameraPosition, LB_InvertedDirection);
    result.Color.rgb = saturate(LinearToGamma(result.Color.rgb));
    
	return result;
}