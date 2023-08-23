#ifndef SHADERFUNCTIONS_HLSLI
#define SHADERFUNCTIONS_HLSLI
#include "../ConstantBuffers/LightBuffer.hlsli"

float4x4 GetSkinMatrix(float4 boneWeights, uint4 boneIDs, float4x4 boneTransforms[128])
{
    float4x4 skinMatrix = 0;
    skinMatrix += boneWeights.x * boneTransforms[boneIDs.x];
    skinMatrix += boneWeights.y * boneTransforms[boneIDs.y];
    skinMatrix += boneWeights.z * boneTransforms[boneIDs.z];
    skinMatrix += boneWeights.w * boneTransforms[boneIDs.w];
    return skinMatrix;
}

float4 GetAlphaBlendColor(float4 first, float4 second)
{
    float4 result = 0;
    result.w = 1 - (1 - first.w) * (1 - second.w);
    
    if (result.w != 0.f)
    {
        float alphaInverse = 1.f / result.w;
        result.x = (first.x * first.w) * alphaInverse + (second.x * second.w) * alphaInverse;
        result.y = (first.y * first.w) * alphaInverse + (second.y * second.w) * alphaInverse;
        result.z = (first.z * first.w) * alphaInverse + (second.z * second.w) * alphaInverse;
    }
    
    return result;
}

float4 GetAdditiveBlendColor(float4 first, float4 second)
{
    float4 result = first;
    result += second;
    return saturate(result);
}

float3 CalculateBlinnPhong(float3 aPixelNormal, float3 aPosition, float3 aColor, float aShininess, float3 aCameraPosition, float3 anInvertedLightDirection)
{
    const float3 V = normalize(aCameraPosition - aPosition);
    const float3 LplusV = anInvertedLightDirection + V;
    const float3 H = normalize((LplusV) / length(LplusV));

    const float LdotN = saturate(dot(anInvertedLightDirection, aPixelNormal));
    const float NdotH = saturate(dot(aPixelNormal, H));

    //const float3 phongA = aColor * LB_AmbientColor;
    const float3 phongD = aColor * (LB_DirectionallightColor * LdotN);
    const float3 phongS = LB_DirectionallightColor * saturate(pow(NdotH, aShininess));

    return /*phongA +*/ phongD + phongS;
}

float3 LinearToGamma(float3 aColor)
{
    return pow(aColor, 0.45454545454545454545454545454545f); // 1 / 2.2
}

uint GetNumMips(TextureCube aCubeMap)
{
    uint width = 0;
    uint height = 0;
    uint numMips = 0;
    aCubeMap.GetDimensions(0, width, height, numMips);
    return numMips;
}

float GetRangeAttenuation(float aDistance, float aLightRange)
{
    return 1 - pow(aDistance / aLightRange, 2);
}

#endif // SHADERFUNCTIONS_HLSLI