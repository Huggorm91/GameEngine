#include "../Default/GBufferData.hlsli"
#include "../Default/PBLFunctions.hlsli"

GBufferOutput main(DefaultVertexToPixel input)
{
    GBufferOutput result;
    
    result.ObjectID = OB_ID;
    
#ifndef _RETAIL
    switch (FB_DebugMode)
    {
        default:
        case 0: // Default
        {
#endif // !_RETAIL
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
                result.FX = FXTexture.Sample(DefaultSampler, scaledUV);
                const float emissionMultiplier = result.FX.r * MB_EmissionIntensity;
                result.Emission.rgb = GetAlphaBlendColor(MB_EmissionColor, float4(textureColor.rgb, 1 - MB_EmissionColor.a)).rgb * emissionMultiplier;
                result.Emission.a = emissionMultiplier;
                result.WorldPosition = input.WorldPosition;
                
#ifndef _RETAIL
                break;
            }
        case 1: // UV
        {
                float2 scaledUV = input.UVs[0] * MB_UVTiling;
                result.Albedo.xy = scaledUV.xy;
                result.Albedo.z = 0;
                result.Albedo.w = 1;
            
                result.Normal = 0;
                result.VertexNormal = 0;
                result.Material = 0;
                result.FX = 0;
                result.Emission = 0;
                result.WorldPosition = 0;
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

                result.Albedo.rgb = (pixelNormal + 1.f) * 0.5f;
                result.Albedo.w = 1;
            
                result.Normal = 0;
                result.VertexNormal = 0;
                result.Material = 0;
                result.FX = 0;
                result.Emission = 0;
                result.WorldPosition = 0;
                break;
            }
        case 3: // Normal
        {
                result.Albedo.rgb = (input.NormalWS + 1.f) * 0.5f;
                result.Albedo.w = 1;
            
                result.Normal = 0;
                result.VertexNormal = 0;
                result.Material = 0;
                result.FX = 0;
                result.Emission = 0;
                result.WorldPosition = 0;
                break;
            }
        case 4: // Tangent
        {
                result.Albedo.rgb = (input.TangentWS + 1.f) * 0.5f;
                result.Albedo.w = 1;
            
                result.Normal = 0;
                result.VertexNormal = 0;
                result.Material = 0;
                result.FX = 0;
                result.Emission = 0;
                result.WorldPosition = 0;
                break;
            }
        case 5: // Binormal
        {
                result.Albedo.rgb = (input.BinormalWS + 1.f) * 0.5f;
                result.Albedo.w = 1;
            
                result.Normal = 0;
                result.VertexNormal = 0;
                result.Material = 0;
                result.FX = 0;
                result.Emission = 0;
                result.WorldPosition = 0;
                break;
            }
        case 6: // AmbientOcclusion
        {
                float2 scaledUV = input.UVs[0] * MB_UVTiling;
                float ambient = MaterialTexture.Sample(DefaultSampler, scaledUV).x;
                result.Albedo.rgb = ambient;
                result.Albedo.w = 1;
            
                result.Normal = 0;
                result.VertexNormal = 0;
                result.Material = 0;
                result.FX = 0;
                result.Emission = 0;
                result.WorldPosition = 0;
                break;
            }
        case 7: // Roughness
        {
                float2 scaledUV = input.UVs[0] * MB_UVTiling;
                float roughness = MaterialTexture.Sample(DefaultSampler, scaledUV).y;
                result.Albedo.rgb = roughness;
                result.Albedo.w = 1;
            
                result.Normal = 0;
                result.VertexNormal = 0;
                result.Material = 0;
                result.FX = 0;
                result.Emission = 0;
                result.WorldPosition = 0;
                break;
            }
        case 8: // Metalness
        {
                float2 scaledUV = input.UVs[0] * MB_UVTiling;
                float metalness = MaterialTexture.Sample(DefaultSampler, scaledUV).z;
                result.Albedo.rgb = metalness;
                result.Albedo.w = 1;
            
                result.Normal = 0;
                result.VertexNormal = 0;
                result.Material = 0;
                result.FX = 0;
                result.Emission = 0;
                result.WorldPosition = 0;
                break;
            }
        case 9: // Emission
        {
                float2 scaledUV = input.UVs[0] * MB_UVTiling;
                float emission = FXTexture.Sample(DefaultSampler, scaledUV).r;
                float4 textureColor = AlbedoTexture.Sample(DefaultSampler, scaledUV);
                //result.Albedo = GetAlphaBlendColor(input.Color[0], textureColor);
                //result.Albedo = GetAlphaBlendColor(result.Albedo, MB_AlbedoColor);
                //result.Albedo *= emission;
                const float emissionMultiplier = emission * MB_EmissionIntensity;
                result.Albedo.rgb = GetAlphaBlendColor(MB_EmissionColor, float4(textureColor.rgb, 1 - MB_EmissionColor.a)).rgb * emissionMultiplier;
                result.Albedo.a = 1;
            
                result.Normal = 0;
                result.VertexNormal = 0;
                result.Material = 0;
                result.FX = 0;
                result.Emission = 0;
                result.WorldPosition = 0;
                break;
            }
        case 10: // VertexColors
        {
                result.Albedo = input.Color[0];
            
                result.Normal = 0;
                result.VertexNormal = 0;
                result.Material = 0;
                result.FX = 0;
                result.Emission = 0;
                result.WorldPosition = 0;
                break;
            }
        case 11: // AlbedoMap
        {
                float2 scaledUV = input.UVs[0] * MB_UVTiling;
                result.Albedo = AlbedoTexture.Sample(DefaultSampler, scaledUV);
            
                result.Normal = 0;
                result.VertexNormal = 0;
                result.Material = 0;
                result.FX = 0;
                result.Emission = 0;
                result.WorldPosition = 0;
                break;
            }
        case 12: // NormalMap
        {
                float2 scaledUV = input.UVs[0] * MB_UVTiling;
                result.Albedo = NormalTexture.Sample(DefaultSampler, scaledUV);
            
                result.Normal = 0;
                result.VertexNormal = 0;
                result.Material = 0;
                result.FX = 0;
                result.Emission = 0;
                result.WorldPosition = 0;
                break;
            }
        case 13: // DirectionallightUV
        {
                float3 uv = GetShadowMapUV(LB_DirectionalView, LB_DirectionalProjection, input.WorldPosition);
                if (uv.x < 0.f || uv.x > 1.f || uv.y < 0.f || uv.y > 1.f)
                {
                    result.Albedo.xy = 0;
                }
                else
                {
                    result.Albedo.xy = uv.xy;
                }
                result.Albedo.z = 0;
                result.Albedo.w = 1;
            
                result.Normal = 0;
                result.VertexNormal = 0;
                result.Material = 0;
                result.FX = 0;
                result.Emission = 0;
                result.WorldPosition = 0;
        
                break;
            }
    }
#endif // !_RETAIL
    
    return result;
}