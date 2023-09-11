#include "../Default/PostProcessingData.hlsli"

float4 main(QuadVSToPS input) : SV_TARGET
{
    float4 result = Intermediate_ATexture.Sample(DefaultSampler, input.UV);
    
    if (result.a < 0.05f)
    {
        discard;
        return result;
    }
    
    const float directions = 16.f;
    const float directionsInverse = 1 / directions;
    
    const float quality = 4.f;
    const float qualityInverse = 1 / quality;
    
    const float radius = 2.f;
    
    const float2 rad = radius / FB_ScreenSize;

    float d = 0.f;
    float f = 0.f;
    [unroll]
    for (int j = 0.f; j < directions; j++)
    {
        f = qualityInverse;
        
        [unroll]
        for (int i = 0; i < quality; i++)
        {
            result += Intermediate_ATexture.Sample(DefaultSampler, input.UV + float2(cos(d), sin(d)) * rad * f);
            f += qualityInverse;
        }
        
        d += PI2 * directionsInverse;
    }
    
    result.rgb /= quality * directions - 15.f;
    result.a = 1;
    return result;
}