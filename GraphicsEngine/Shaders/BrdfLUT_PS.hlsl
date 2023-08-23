#include "Default/BrdfData.hlsli"
float2 main(BrdfVSToPS input) : SV_TARGET
{
    return IntegrateBRDF(input.UV.x, input.UV.y);
}