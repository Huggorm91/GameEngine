#include "../Default/PostProcessingData.hlsli"

float4 main(QuadVSToPS input) : SV_TARGET
{
    return Intermediate_ATexture.Sample(BlurSampler, input.UV);
}