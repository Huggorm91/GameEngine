#include "../Default/PostProcessingData.hlsli"

float4 main(QuadVSToPS input) : SV_TARGET
{
    float4 result = Intermediate_ATexture.Sample(DefaultSampler, input.UV);
    if (result.a <= 0.f)
    {
        discard;
        return result;
    }
    result.rgb = saturate(LinearToGamma(result.rgb));
    return result;
}