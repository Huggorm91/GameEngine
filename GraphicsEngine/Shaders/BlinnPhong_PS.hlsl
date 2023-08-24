#include "Default/PhongFunctions.hlsli"

DefaultPixelOutput main(DefaultVertexToPixel input)
{
    DefaultPixelOutput result;
    
#ifdef _DEBUG
    switch(FB_DebugMode)
    {
    default:
    case 0:
    {
#endif
    
    float2 scaledUV = input.UVs[0] * MB_UVTiling;
    float4 textureColor = AlbedoTexture.Sample(DefaultSampler, scaledUV);
    result.Color = GetAlphaBlendColor(input.Color[0], textureColor);
    result.Color = GetAlphaBlendColor(result.Color, MB_AlbedoColor);
    //result.Color = GetAdditiveBlendColor(input.Color[0], textureColor);

    float3 pixelNormal = NormalTexture.Sample(DefaultSampler, scaledUV).rgb;
    pixelNormal.xy = 2 * pixelNormal.xy - 1;
    pixelNormal.z = sqrt(1 - saturate(pixelNormal.x * pixelNormal.x + pixelNormal.y * pixelNormal.y));
    pixelNormal.xy *= MB_NormalStrength;
    pixelNormal = normalize(mul(pixelNormal, float3x3(input.TangentWS, input.BinormalWS, input.NormalWS)));

    //result.Color.rgb = CalculateBlinnPhong(pixelNormal, input.Position.xyz, result.Color.rgb, 62, FB_CameraPosition, LB_InvertedDirection);
    result.Color.rgb = GetBlinnPhongLight(input.WorldPosition.xyz, pixelNormal, result.Color.rgb);
    
    result.Color.rgb = saturate(LinearToGamma(result.Color.rgb));
    
#ifdef _DEBUG
        break;
    }
    case 1:
    {
        float2 scaledUV = input.UVs[0] * MB_UVTiling;
        result.Color.x = scaledUV.x;
        result.Color.y = scaledUV.y;
        result.Color.z = 0;
        result.Color.w = 1;
        break;
    }
    case 2:
    {
        const float3x3 tbn = float3x3(input.TangentWS, input.BinormalWS, input.NormalWS);

        float3 pixelNormal = NormalTexture.Sample(DefaultSampler, input.UVs[0]).rgb;
        pixelNormal.xy = 2 * pixelNormal.xy - 1;
        pixelNormal.z = sqrt(1 - saturate(pixelNormal.x * pixelNormal.x + pixelNormal.y * pixelNormal.y));
        pixelNormal.xy *= MB_NormalStrength;
        pixelNormal = normalize(mul(pixelNormal, tbn));

        result.Color.rgb = (pixelNormal + 1.f) * 0.5f;
        result.Color.w = 1;
        break;
    }
    case 3:
    {
        result.Color.rgb = (input.NormalWS + 1.f) * 0.5f;
        result.Color.w = 1;
        break;
    }
    case 4:
    {
        result.Color.rgb = (input.TangentWS + 1.f) * 0.5f;
        result.Color.w = 1;
        break;
    }
    case 5:
    {
        result.Color.rgb = (input.BinormalWS + 1.f) * 0.5f;
        result.Color.w = 1;
        break;
    }
    case 6:
    {
        // AmbientOcclusion is not used by BlinnPhong
        result.Color.rgb = 0;
        result.Color.w = 1;
        break;
    }
    case 7:
    {
        result.Color.rgb = MB_Shininess;
        result.Color.w = 1;
        break;
    }
    case 8:
    {
        result.Color.rgb = MB_Metalness;
        result.Color.w = 1;
        break;
    }
    case 9:
    {
        result.Color = input.Color[0];
        break;
    }
    case 10:
    {
        float2 scaledUV = input.UVs[0] * MB_UVTiling;
        result.Color = AlbedoTexture.Sample(DefaultSampler, scaledUV);
        break;
    }
    case 11:
    {
        float2 scaledUV = input.UVs[0] * MB_UVTiling;
        result.Color = NormalTexture.Sample(DefaultSampler, scaledUV);
        break;
    }
    }
#endif
    
    return result;
}