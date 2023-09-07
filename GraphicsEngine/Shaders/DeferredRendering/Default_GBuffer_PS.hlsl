#include "../Default/GBufferData.hlsli"
GBufferOutput main(DefaultVertexToPixel input)
{
    GBufferOutput result;
    
    float2 scaledUV = input.UVs[0] * MB_UVTiling;
    float4 textureColor = AlbedoTexture.Sample(DefaultSampler, scaledUV);
    result.Albedo = GetAlphaBlendColor(input.Color[0], textureColor);
    result.Albedo = GetAlphaBlendColor(result.Albedo, MB_AlbedoColor);
    //result.Color = GetAdditiveBlendColor(input.Color[0], textureColor);

    float3 pixelNormal = NormalTexture.Sample(DefaultSampler, scaledUV).rgb;
    pixelNormal.xy = 2 * pixelNormal.xy - 1;
    pixelNormal.z = sqrt(1 - saturate(pixelNormal.x * pixelNormal.x + pixelNormal.y * pixelNormal.y));
    pixelNormal.xy *= MB_NormalStrength;
    pixelNormal = normalize(mul(pixelNormal, float3x3(input.TangentWS, input.BinormalWS, input.NormalWS)));
    
    result.Normal.xyz = pixelNormal;
    result.Normal.w = 0;
    result.VertexNormal.xyz = input.NormalWS;
    result.VertexNormal.w = 0;
    result.Material = MaterialTexture.Sample(DefaultSampler, scaledUV);
    result.WorldPosition = input.WorldPosition;
    result.ObjectID = OB_ID;
    return result;
}