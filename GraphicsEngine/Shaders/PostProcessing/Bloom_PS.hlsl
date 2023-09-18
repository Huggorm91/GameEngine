#include "../Default/PostProcessingData.hlsli"

float4 main(QuadVSToPS input) : SV_TARGET
{
    float4 result;
    const float3 scene = Intermediate_ATexture.Sample(DefaultSampler, input.UV).rgb;
    const float3 blur = Intermediate_BTexture.Sample(DefaultSampler, input.UV).rgb;
    
    const float3 scaledBlur = (scene * 0.1f) * (1.f - saturate(blur));
    
    result.rgb = scaledBlur + blur;
    result.a = 1;
    return result;
}