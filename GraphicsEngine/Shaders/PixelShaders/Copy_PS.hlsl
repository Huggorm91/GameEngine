#include "../Default/QuadData.hlsli"
Texture2D AlbedoTexture : register(t0);
SamplerState DefaultSampler : register(s0);

float4 main(QuadVSToPS input) : SV_TARGET
{
    return AlbedoTexture.Sample(DefaultSampler, input.UV);
}