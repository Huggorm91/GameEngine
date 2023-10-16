#include "../Default/PostProcessingData.hlsli"

float4 main(QuadVSToPS input) : SV_TARGET
{
    float4 result;
    const float4 scene = Intermediate_ATexture.Sample(DefaultSampler, input.UV);
    const float4 blur = Intermediate_BTexture.Sample(BlurSampler, input.UV);
    
    // Additive
    //{ 
    //    result.rgb = scene + blur;
    //}
    
    // Scaled
    {
        const float3 scaledBlur = scene.rgb * (1.f - saturate(blur.rgb));
        result.rgb = scaledBlur + blur.rgb;
    }
    
    // Luminance Based
    //{
    //    const float luminace = dot(scene.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    //    const float3 scaledBlur = blur.rgb * (1.0f - luminace);
    //    result.rgb = scene.rgb + scaledBlur;
    //}    
    
    result.a = blur.a + scene.a;
    return result;
}