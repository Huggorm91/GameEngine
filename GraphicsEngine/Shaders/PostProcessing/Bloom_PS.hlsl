#include "../Default/PostProcessingData.hlsli"

float4 main(QuadVSToPS input) : SV_TARGET
{
    float4 result;
    const float3 blur1 = Intermediate_ATexture.Sample(DefaultSampler, input.UV).rgb;
    const float3 blur2 = Intermediate_BTexture.Sample(DefaultSampler, input.UV).rgb;
    
    const float3 scaledBlur = blur1 * (1.f - saturate(blur2));
    
    result.rgb = scaledBlur + blur2;
    result.a = 1;
    return result;
}