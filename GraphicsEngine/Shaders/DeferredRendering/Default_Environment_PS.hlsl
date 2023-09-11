#include "../Default/GBufferData.hlsli"
#include "../Default/PBLFunctions.hlsli"

DefaultPixelOutput main(QuadVSToPS input)
{
    DefaultPixelOutput result;
    result.Color = 0;
    
    const float4 albedo = GBuffer_Albedo.Sample(DefaultSampler, input.UV);
    
    if (albedo.a <= 0.f)
    {
        discard;
        return result;
    }

#ifndef _RETAIL
    switch (FB_DebugMode)
    {
        default:
        case 0:
        {
                if (FB_LightMode == 5)
                {
                    result.Color = albedo;
                    break;
                }
                if (FB_LightMode == 3 || FB_LightMode == 4)
                {
                    result.Color = 0;
                    break;
                }
#endif // !_RETAIL
                const float3 material = GBuffer_Material.Sample(DefaultSampler, input.UV).rgb;
                //const float3 vertexNormal = GBuffer_VertexNormal.Sample(DefaultSampler, input.UV).rgb;
                const float3 pixelNormal = GBuffer_PixelNormal.Sample(DefaultSampler, input.UV).rgb;
                const float4 worldPosition = GBuffer_Position.Sample(DefaultSampler, input.UV);
                const float emission = GBuffer_FX.Sample(DefaultSampler, input.UV).r;
            
                LightData data;
                data.position = worldPosition;
                data.pixelNormal = pixelNormal;
                data.roughness = material.g;
                data.metalness = material.b;
    
                const float3 diffuseColor = lerp((float3) 0.f, albedo.rgb, 1 - data.metalness);
                data.v = normalize(FB_CameraPosition - (float3) data.position);
                data.brdfDiffuse = diffuseColor * PI_INVERSE;
                data.specularColor = lerp((float3) 0.04f, albedo.rgb, data.metalness);
                data.invertedSpecular = 1 - data.specularColor;
                const float a = Pow2(data.roughness);
                data.aPow2 = Pow2(a);
                data.k = a * 0.125f; // a / 8
                data.kInverse = 1 - data.k;
                data.nDotV = saturate(dot(data.pixelNormal, data.v));
    
#ifndef _RETAIL
                if (FB_LightMode == 0 || FB_LightMode == 1)
                {
#endif // !_RETAIL
                    result.Color.rgb = GetPblAmbientlight(data, material.r, diffuseColor);
#ifndef _RETAIL
                }
                if (FB_LightMode == 0 || FB_LightMode == 2)
                {
#endif // !_RETAIL
                    result.Color.rgb += GetPblDirectionallight(data);
#ifndef _RETAIL
                }
#endif // !_RETAIL
                result.Color.rgb += albedo.rgb * emission;
                result.Color.rgb = saturate(LinearToGamma(result.Color.rgb));
                result.Color.a = albedo.a;
#ifndef _RETAIL
                break;
            }
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        {    
                result.Color = albedo;
                break;
            }
    }
#endif // !_RETAIL
    
    return result;
}