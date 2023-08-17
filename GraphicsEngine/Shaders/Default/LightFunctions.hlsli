#ifndef LIGHTFUNCTIONS_HLSLI
#define LIGHTFUNCTIONS_HLSLI
#include "../ConstantBuffers/FrameBuffer.hlsli"
#include "../ConstantBuffers/LightBuffer.hlsli"
#include "../ConstantBuffers/MaterialBuffer.hlsli"

float3 GetPointlightValue(float3 aKSMultiplier, float aNDotInvDir, PointlightData aPointLight)
{
//   // const float3 cubeMap = environmentCube.Sample(defaultSampler, inverseLightDirection);
    
//    float3 kD = saturate(aPointLight.DiffuseColor * aNDotInvDir);
    
//    float3 kS = aPointLight.SpecularColor * aKSMultiplier * aPointLight.Intensity);
    
//    float aPL; //Light attenuation
//    float d = distance(aPosition, aPointLight.Position); //Distance between the light and the pixel
//    float lightRange = max(aPointLight.Range, 0.00001f);
//    aPL = saturate(1 - pow(d * (1 / lightRange), 2));
    
//    return (kD + kS) * aPL * aPointLight.Intensity;
    return 0;
}

float3 GetSpotLightValue(float3 aKSMultiplier, float aNDotInvDir, SpotlightData aSpotLight)
{
//    //const float3 cubeMap = environmentCube.Sample(defaultSampler, aSpotLight.Direction);
    
//    float3 kD = saturate(aSpotLight.DiffuseColor * aNDotInvDir);
    
//    float3 kS = aSpotLight.SpecularColor * aKSMultiplier * aSpotLight.Intensity;
    
//    float aLR; //Light range attenuation
//    float d = distance(aPosition, aSpotLight.Position); //Distance between the light and the pixel
//    float lightRange = max(aSpotLight.Range, 0.00001f);
//    aLR = 1 - pow(d * (1.0f / lightRange), 2.0f);
   
//    float aLW; //Light width attenuation
//    float aDot = dot(-aSpotLight.Direction, inverseLightDirection);
//    float dotMinSin = saturate(aDot - cos(aSpotLight.OuterConeAngle));
//    float otherCos = cos(aSpotLight.InnerConeAngle) - cos(aSpotLight.OuterConeAngle);
    
//    aLW = pow((dotMinSin / otherCos), 2);
    
//    //float aSL = pow((dot(-aSpotLight.Direction, inverseLightDirection)) - sin(aSpotLight.OuterConeAngle) / (cos(aSpotLight.InnerConeAngle) - cos(aSpotLight.OuterConeAngle)), 2);
    
//    float aSL = saturate(aLW * aLR); //Spot Light attenuation
    
//    return (kD + kS) * aSL * aSpotLight.Intensity;
    return 0;
}

float3 GetDirectionallight(float3 aKSMultiplier, float aNDotInvDir)
{    
    //const int cubeMips = GetNumMips(environmentCube);
    //const float3 cubeMap = environmentCube.Sample(defaultSampler, DirectionalLight.Direction);
    
    float3 kD = saturate(LB_DirectionallightColor * aNDotInvDir);
    float3 kS = LB_DirectionallightColor * aKSMultiplier;
    return kD + kS;
}

float3 GetPointlights(float3 aKSMultiplier, float aNDotInvDir)
{
    float3 result = 0;
    [unroll]
    for (int i = 0; i < 8; i++)
    {
        [flatten]
        if (LB_Pointlights[i].Intensity > 0)
        {
            result += GetPointlightValue(aKSMultiplier, aNDotInvDir, LB_Pointlights[i]);
        }
    }
    return result;
}

float3 GetSpotlights(float3 aKSMultiplier, float aNDotInvDir)
{
    float3 result = 0;
    [unroll]
    for (int i = 0; i < 8; i++)
    {
        [flatten]
        if (LB_Spotlights[i].Intensity > 0)
        {
            result += GetSpotLightValue(aKSMultiplier, aNDotInvDir, LB_Spotlights[i]);
        }
    }
    return result;
}

float3 GetLightSourceContribution(float3 aPosition, float3 aPixelNormal)
{
    float3 result = 0;
    
    const float3 v = normalize(FB_CameraPosition - aPosition);
    const float3 invertedDirectionPlusV = LB_InvertedDirection + v;
    const float3 h = invertedDirectionPlusV / length(invertedDirectionPlusV);
    const float nDotH = saturate(dot(aPixelNormal, h));
    
    const float nDotInvDir = dot(aPixelNormal, LB_InvertedDirection);
    const float3 kSMultiplier = pow(nDotH, MB_Shininess);
    
    result = GetDirectionallight(kSMultiplier, nDotInvDir);
    result += GetPointlights(kSMultiplier, nDotInvDir);
    result += GetSpotlights(kSMultiplier, nDotInvDir);
    return result;
}

#endif // LIGHTFUNCTIONS_HLSLI