#include "../Default/PBLFunctions.hlsli"

DefaultPixelOutput main(DefaultVertexToPixel input)
{
    DefaultPixelOutput result;
    
#ifndef _RETAIL
    switch (FB_DebugMode)
    {
        default:
        case 0: // Default
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
    
                float3 materialMap = MaterialTexture.Sample(DefaultSampler, scaledUV).rgb;
    
                LightData data;
                data.position = input.WorldPosition;
                data.pixelNormal = pixelNormal;
                data.roughness = materialMap.g;
                data.metalness = materialMap.b;
    
    // These are calculated inside GetPblLight()
                data.v = 0;
                data.brdfDiffuse = 0;
                data.specularColor = 0;
                data.invertedSpecular = 0;
                data.k = 0;
                data.kInverse = 0;
                data.aPow2 = 0;
                data.nDotV = 0;
    
                result.Color.rgb = GetPblLight(data, result.Color.rgb, materialMap.r);
    
                result.Color.rgb = saturate(LinearToGamma(result.Color.rgb));
#ifndef _RETAIL
                break;
            }
        case 1: // UV
    {
                float2 scaledUV = input.UVs[0] * MB_UVTiling;
                result.Color.xy = scaledUV.xy;
                result.Color.z = 0;
                result.Color.w = 1;
                break;
            }
        case 2: // PixelNormal
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
        case 3: // Normal
    {
                result.Color.rgb = (input.NormalWS + 1.f) * 0.5f;
                result.Color.w = 1;
                break;
            }
        case 4: // Tangent
    {
                result.Color.rgb = (input.TangentWS + 1.f) * 0.5f;
                result.Color.w = 1;
                break;
            }
        case 5: // Binormal
    {
                result.Color.rgb = (input.BinormalWS + 1.f) * 0.5f;
                result.Color.w = 1;
                break;
            }
        case 6: // AmbientOcclusion
    {
                float2 scaledUV = input.UVs[0] * MB_UVTiling;
                float ambient = MaterialTexture.Sample(DefaultSampler, scaledUV).x;
                result.Color.rgb = ambient;
                result.Color.w = 1;
                break;
            }
        case 7: // Roughness
    {
                float2 scaledUV = input.UVs[0] * MB_UVTiling;
                float roughness = MaterialTexture.Sample(DefaultSampler, scaledUV).y;
                result.Color.rgb = roughness;
                result.Color.w = 1;
                break;
            }
        case 8: // Metalness
    {
                float2 scaledUV = input.UVs[0] * MB_UVTiling;
                float metalness = MaterialTexture.Sample(DefaultSampler, scaledUV).z;
                result.Color.rgb = metalness;
                result.Color.w = 1;
                break;
            }
        case 9: // Emission
        {
                float2 scaledUV = input.UVs[0] * MB_UVTiling;
                float emission = FXTexture.Sample(DefaultSampler, scaledUV).r;
                float4 textureColor = AlbedoTexture.Sample(DefaultSampler, scaledUV);
                result.Color = GetAlphaBlendColor(input.Color[0], textureColor);
                result.Color = GetAlphaBlendColor(result.Color, MB_AlbedoColor);
                result.Color *= emission;
                result.Color.w = 1;
                break;
            }
        case 10: // VertexColors
    {
                result.Color = input.Color[0];
                break;
            }
        case 11: // AlbedoMap
    {
                float2 scaledUV = input.UVs[0] * MB_UVTiling;
                result.Color = AlbedoTexture.Sample(DefaultSampler, scaledUV);
                break;
            }
        case 12: // NormalMap
    {
                float2 scaledUV = input.UVs[0] * MB_UVTiling;
                result.Color = NormalTexture.Sample(DefaultSampler, scaledUV);
                break;
            }
        case 13: // DirectionallightUV
    {
                float3 uv = GetShadowMapUV(LB_DirectionalView, LB_DirectionalProjection, input.WorldPosition);
                if (uv.x < 0.f || uv.x > 1.f || uv.y < 0.f || uv.y > 1.f)
                {
                    result.Color.xyz = 0;
                    result.Color.w = 1;
                }
                else
                {
                    result.Color.xy = uv.xy;
                    result.Color.z = 0;
                    result.Color.w = 1;
                }
        
                break;
            }
    }
#endif
    
    return result;
}