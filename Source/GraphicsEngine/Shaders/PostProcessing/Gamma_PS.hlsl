#include "../Default/PostProcessingData.hlsli"
#include "../Default/ToneMaps.hlsli"

float4 main(QuadVSToPS input) : SV_TARGET
{
    float4 result = Intermediate_ATexture.Sample(DefaultSampler, input.UV);
    if (result.a <= 0.f)
    {
        discard;
        return result;
    }
    switch (LB_ToneMapMode)
    {
        default: // case 0 is Default
        case 0:
            {
                result.rgb = LinearToGamma(result.rgb);
                break;
            }
        case 1:
            {
                result.rgb = LinearToGamma(Tonemap_Reinhard2(result.rgb));
                break;
            }
        case 2:
            {
                result.rgb = Tonemap_UnrealEngine(result.rgb);
                break;
            }
        case 3:
            {
                result.rgb = LinearToGamma(Tonemap_ACES(result.rgb));
                break;
            }
        case 4:
            {
                result.rgb = LinearToGamma(Tonemap_Lottes(result.rgb));
                break;
            }
    }
    result.a = 1.f;
    
    return result;
}