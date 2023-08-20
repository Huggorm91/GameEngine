#include "Default/DefaultMaterialData.hlsli"

DefaultPixelOutput main(DefaultVertexToPixel input)
{
	DefaultPixelOutput result;

    float2 scaledUV = input.UVs[0] * MB_UVTiling;
    float4 textureColor = AlbedoTexture.Sample(DefaultSampler, scaledUV);
	result.Color = GetAlphaBlendColor(input.Color[0], textureColor);
    result.Color = GetAlphaBlendColor(result.Color, MB_AlbedoColor);
    //result.Color = GetAdditiveBlendColor(input.Color[0], textureColor);

    float3 pixelNormal = NormalTexture.Sample(DefaultSampler, scaledUV);
    pixelNormal.xy = 2 * pixelNormal.xy - 1;
    pixelNormal.z = sqrt(1 - saturate(pixelNormal.x * pixelNormal.x + pixelNormal.y * pixelNormal.y));
    pixelNormal.xy *= MB_NormalStrength;
    pixelNormal = normalize(mul(pixelNormal, float3x3(input.TangentWS, input.BinormalWS, input.NormalWS)));

    //result.Color.rgb = CalculateBlinnPhong(pixelNormal, input.Position.xyz, result.Color.rgb, 62, FB_CameraPosition, LB_InvertedDirection);
    result.Color.rgb = GetLightSourceContribution(input.WorldPosition.xyz, pixelNormal, result.Color.rgb);
    
    result.Color.rgb = saturate(LinearToGamma(result.Color.rgb));
    
	return result;
}