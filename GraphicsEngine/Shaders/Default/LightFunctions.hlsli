#ifndef LIGHTFUNCTIONS_HLSLI
#define LIGHTFUNCTIONS_HLSLI
#include "../ConstantBuffers/FrameBuffer.hlsli"
#include "../ConstantBuffers/LightBuffer.hlsli"
#include "../ConstantBuffers/MaterialBuffer.hlsli"

float3 GetPointlightValue(float3 aPosition, float3 aPixelNormal, float3 aV, PointlightData aPointLight, float3 aColor)
{  
    //const float3 invertedDirection = (aSpotLight.Position - aPosition);
    //const float nDotL = dot(aPixelNormal, invertedDirection);

    //const float3 LplusV = invertedDirection + aV;
    //const float3 h = normalize(LplusV / length(LplusV));
    //const float nDotH = saturate(dot(aPixelNormal, h));
    
    //float d = distance(aPosition, aPointLight.Position);
    //float lightRange = max(aPointLight.Radius, 0.00001f);
    
    //float3 kD = aColor * (aPointLight.Color * nDotL);
    //float3 kS = aPointLight.Color * pow(nDotH, MB_Shininess) * aPointLight.Intensity;
    //float a = saturate(1 - pow(d * (1 / lightRange), 2));   
    
    //return (kD + kS) * a * aPointLight.Intensity;
    return 0;
}

float3 GetSpotLightValue(float3 aPosition, float3 aPixelNormal, float3 aV, SpotlightData aSpotLight, float3 aColor)
{
    //const float3 invertedDirection = (aSpotLight.Position - aPosition);
    //const float nDotL = dot(aPixelNormal, invertedDirection);

    //const float3 LplusV = invertedDirection + aV;
    //const float3 h = normalize(LplusV / length(LplusV));
    //const float nDotH = saturate(dot(aPixelNormal, h));
    
    //float3 kD = aColor * (aSpotLight.Color * nDotL);
    //float3 kS = aSpotLight.Color * pow(nDotH, MB_Shininess) * aSpotLight.Intensity;
    
    //float a = 1 - pow(d * (1.0f / lightRange), 2.0f);
    //float d = distance(aPosition, aSpotLight.Position);
    //float lightRange = max(aSpotLight.Range, 0.00001f);
   
    //float aLW; //Light width attenuation
    //float aDot = dot(-aSpotLight.Direction, invertedDirection);
    //float dotMinSin = saturate(aDot - cos(aSpotLight.OuterConeAngle));
    //float otherCos = cos(aSpotLight.InnerConeAngle) - cos(aSpotLight.OuterConeAngle);
    
    //aLW = pow((dotMinSin / otherCos), 2);
    
    ////float aSL = pow((dot(-aSpotLight.Direction, inverseLightDirection)) - sin(aSpotLight.OuterConeAngle) / (cos(aSpotLight.InnerConeAngle) - cos(aSpotLight.OuterConeAngle)), 2);
    
    //float aSL = saturate(aLW * aLR); //Spot Light attenuation
    
    //return (kD + kS) * aSL * aSpotLight.Intensity;
    return 0;
}

float3 GetDirectionallight(float3 aPosition, float3 aPixelNormal, float3 aV, float3 aColor)
{
    const float nDotL = saturate(dot(aPixelNormal, LB_InvertedDirection));

    const float3 LplusV = LB_InvertedDirection + aV;
    const float3 h = normalize(LplusV / length(LplusV));
    const float nDotH = saturate(dot(aPixelNormal, h));
    
    float3 kD = aColor * (LB_DirectionallightColor * nDotL);
    float3 kS = LB_DirectionallightColor * saturate(pow(nDotH, MB_Shininess));
    return (kD + kS) * LB_DirectionallightIntensity;
}

float3 GetPointlights(float3 aPosition, float3 aPixelNormal, float3 aV, float3 aColor)
{
    float3 result = 0;
    [unroll]
    for (int i = 0; i < 8; i++)
    {
        [flatten]
        if (LB_Pointlights[i].Intensity > 0)
        {
            result += GetPointlightValue(aPosition, aPixelNormal, aV, LB_Pointlights[i], aColor);
        }
    }
    return result;
}

float3 GetSpotlights(float3 aPosition, float3 aPixelNormal, float3 aV, float3 aColor)
{
    float3 result = 0;
    [unroll]
    for (int i = 0; i < 8; i++)
    {
        [flatten]
        if (LB_Spotlights[i].Intensity > 0)
        {
            result += GetSpotLightValue(aPosition, aPixelNormal, aV, LB_Spotlights[i], aColor);
        }
    }
    return result;
}

float3 GetLightSourceContribution(float3 aPosition, float3 aPixelNormal, float3 aColor)
{
    float3 result = 0;
    
    ////IBL (Image Based Lighting) - ambiance and reflections
    //float3 v = normalize(FB_Pos.rgb - aInput.WorldPosition.rgb);
    //const int cubeMips = GetNumMips(environmentCube);
    //float mipLvl = (cubeMips / Material.Shininess) * cubeMips;
    ////mipLvl += abs(length(FB_Pos.rgb - aInput.WorldPosition.rgb))/600;
    //clamp(mipLvl, 0, cubeMips - 1);
    //float3 reflectionVector = normalize(reflect(-v.rgb, pixelNormal.rgb));
    //const float3 reflectionColor = environmentCube.SampleLevel(defaultSampler, reflectionVector, mipLvl).rgb;
    
    const float3 v = normalize(FB_CameraPosition - aPosition);
    
    result = GetDirectionallight(aPosition, aPixelNormal, v, aColor);
    //result += GetPointlights(kSMultiplier, nDotL, aColor);
    //result += GetSpotlights(kSMultiplier, nDotL, aColor);
    return result;
}

#endif // LIGHTFUNCTIONS_HLSLI