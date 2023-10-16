#include "../Default/PostProcessingData.hlsli"
#include "../Default/GBufferData.hlsli"

float4 main(QuadVSToPS input) : SV_TARGET
{
    const float4 albedo = Intermediate_ATexture.Sample(DefaultSampler, input.UV);
    if (albedo.a <= 0.f)
    {
        discard;
        return float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    
    float4 result = 0;
    
    // Gain, no Cutoff.
    //{
    //    const float luminance = dot(albedo.rgb, float3(0.2126f, 0.7152f, .0722f));
    //    result.rgb = albedo * pow(luminance, 5);
    //}
    
    // Flat cutoff
    //{
    //    const float luminance = dot(albedo.rgb, float3(0.2126f, 0.7152f, .0722f));
    //    float cutOff = 0.7f;
        
    //    if (luminance >= cutOff)
    //    {
    //        result.rgb = luminance;
    //    }
    //    else
    //    {
    //        result.rgb = 0;
    //    }
    //}
    
    // Gain, with cutoff.
    {
        const float luminance = dot(albedo.rgb, float3(0.2126f, 0.7152f, 0.0722f));
        const float cutOff = 0.5f;
    
        result.rgb = albedo.rgb * saturate(luminance - cutOff) / cutOff;
    }
    
    // Cutoff with Fade
    //{
    //    const float luminance = dot(albedo.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    //    const float cutOff = 0.8f;
    //    const float fadeLimit = 0.6f;
    
    //    if (luminance >= cutOff)
    //    {
    //        result.rgb = albedo.rgb;
    //    }
    //    else if (luminance >= fadeLimit)
    //    {
    //        float fade = luminance / cutOff;
    //        fade = pow(fade, 5);
    //        result.rgb = albedo.rgb * fade;
    //    }
    //    else
    //    {
    //        result.rgb = 0;
    //    }
    //}
    
    result.rgb = result.rgb + GBuffer_Emission.Sample(DefaultSampler, input.UV).rgb;
    result.a = 1.f;
    return result;
}