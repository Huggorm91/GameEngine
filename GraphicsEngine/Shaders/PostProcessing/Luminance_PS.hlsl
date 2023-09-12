#include "../Default/PostProcessingData.hlsli"

float4 main(QuadVSToPS input) : SV_TARGET
{
    const float3 albedo = Intermediate_ATexture.Sample(DefaultSampler, input.UV).rgb;
    const float luminance = dot(albedo, float3(0.2126f, 0.7152f, 0.0722f));
    const float cutOff = 0.7f;
    const float fadeLimit = 0.4f;
    
    if(luminance >= cutOff)
    {
        return float4(albedo, 1.f);
    }
    else if (luminance >= fadeLimit)
    {
        float fade = luminance / cutOff;
        fade = pow(fade, 5);
        return float4(albedo * fade, 1.f);
    }
    else
    {
        return float4(0.0f, 0.0f, 0.0f, 0.0f);
    }	
}