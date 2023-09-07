#include "Default/BrdfData.hlsli"
float2 main(QuadVSToPS input) : SV_TARGET
{
    return IntegrateBRDF(input.UV.x, input.UV.y);
}