#include "../Default/GBufferData.hlsli"
#include "../Default/PBLFunctions.hlsli"

DefaultPixelOutput main(QuadVSToPS input)
{
    DefaultPixelOutput result;
    
    const float4 albedo = GBuffer_Albedo.Sample(DefaultSampler, input.UV);
    
    if (albedo.a <= 0.f)
    {
        discard;
        result.Color = 0;
        return result;
    }
    
    const float3 material = GBuffer_Material.Sample(DefaultSampler, input.UV).rgb;
    //const float3 vertexNormal = GBuffer_VertexNormal.Sample(DefaultSampler, input.UV).rgb;
    const float3 pixelNormal = GBuffer_PixelNormal.Sample(DefaultSampler, input.UV).rgb;
    const float4 worldPosition = GBuffer_Position.Sample(DefaultSampler, input.UV);
    //const float emission = GBuffer_FX.Sample(DefaultSampler, input.UV).r;
    
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
    
    result.Color.rgb = GetPblSpotlightValue(data, LB_Spotlights[0]);
    
    result.Color.a = saturate(length(result.Color.rgb) * 1000.f);

    return result;
}