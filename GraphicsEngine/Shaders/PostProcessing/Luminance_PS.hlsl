#include "../Default/PostProcessingData.hlsli"

float4 main(QuadVSToPS input) : SV_TARGET
{
    const float4 albedo = Intermediate_ATexture.Sample(DefaultSampler, input.UV);
    if (albedo.a <= 0.f)
    {
        discard;
        return float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    
    const float luminance = dot(albedo.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    const float cutOff = 0.5f;
    
    return float4(albedo.rgb * saturate(luminance - cutOff) / cutOff, 1.f);
    //const float fadeLimit = 0.25f;
    
    //if(luminance >= cutOff)
    //{
    //    return float4(albedo.rgb, 1.f);
    //}
    //else if (luminance >= fadeLimit)
    //{
    //    float fade = luminance / cutOff;
    //    fade = pow(fade, 5);
    //    return float4(albedo.rgb * fade, 1.f);
    //}
    //else
    //{
    //    return float4(0.0f, 0.0f, 0.0f, 0.0f);
    //}	
}