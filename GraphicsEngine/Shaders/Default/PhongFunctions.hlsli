#ifndef PHONGFUNCTIONS_HLSLI
#define PHONGFUNCTIONS_HLSLI
#include "DefaultMaterialData.hlsli"

float3 GetPhongPointlightValue(float3 aPosition, float3 aPixelNormal, float3 aV, PointlightData aPointLight, float3 aColor)
{
    const float3 invertedDirection = normalize(aPointLight.Position - aPosition);
    const float nDotL = max(dot(aPixelNormal, invertedDirection), 0.f);

    const float3 LplusV = invertedDirection + aV;
    const float3 h = normalize(LplusV / length(LplusV));
    const float nDotH = saturate(dot(aPixelNormal, h));
    
    const float3 kD = aColor * aPointLight.Color * nDotL;
    const float3 kS = lerp(aPointLight.Color, aColor, MB_Metalness) * pow(nDotH, MB_Shininess);
    const float attenuation = saturate(GetRangeAttenuation(distance(aPosition, aPointLight.Position), max(aPointLight.Radius, 0.00001f)));
    
    return (kD + kS) * attenuation * aPointLight.Intensity;
}

float3 GetPhongSpotLightValue(float3 aPosition, float3 aPixelNormal, float3 aV, SpotlightData aSpotLight, float3 aColor)
{
    const float3 invertedDirection = normalize(aSpotLight.Position - aPosition);
    const float nDotL = max(dot(aPixelNormal, invertedDirection), 0.f);

    const float3 LplusV = invertedDirection + aV;
    const float3 h = normalize(LplusV / length(LplusV));
    const float nDotH = saturate(dot(aPixelNormal, h));
    
    const float3 kD = aColor * aSpotLight.Color * nDotL;
    const float3 kS = lerp(aSpotLight.Color, aColor, MB_Metalness) * pow(nDotH, MB_Shininess);
    
    const float widthAttenuation = saturate((dot(-aSpotLight.LightDirection, invertedDirection) - sin(aSpotLight.OuterAngle)) / max(cos(aSpotLight.InnerAngle) - cos(aSpotLight.OuterAngle), 0.00001f));
    const float attenuation = saturate(widthAttenuation * GetRangeAttenuation(distance(aPosition, aSpotLight.Position), max(aSpotLight.Range, 0.00001f)));
    
    return (kD + kS) * attenuation * aSpotLight.Intensity;
}

float3 GetPhongAmbientlight(float3 aPixelNormal, float3 aV, float3 aColor)
{
    const float3 reflection = normalize(reflect(-aV, aPixelNormal));

    const uint cubeMips = GetNumMips(EnvironmentCubeMap);
    float mipLvl = clamp(cubeMips * (1 - MB_Shininess * 0.001f), 0, cubeMips - 1);
    const float3 ambientKD = EnvironmentCubeMap.SampleLevel(DefaultSampler, aPixelNormal, cubeMips - 1).rgb;
    const float3 ambientKS = EnvironmentCubeMap.SampleLevel(DefaultSampler, reflection, mipLvl).rgb;
    const float3 ambientLight = (ambientKD + ambientKS) * LB_AmbientlightIntensity;
    
    return aColor * ambientLight;
}

float3 GetPhongDirectionallight(float3 aPosition, float3 aPixelNormal, float3 aV, float3 aColor)
{
    const float nDotL = max(dot(aPixelNormal, LB_InvertedDirection), 0.f);

    const float3 LplusV = LB_InvertedDirection + aV;
    const float3 h = normalize(LplusV / length(LplusV));
    const float nDotH = max(dot(aPixelNormal, h), 0.f);
    
    //const float3 r = reflect(-LB_InvertedDirection, aPixelNormal);
    //const float rDotv = saturate(dot(r, aV));
    
    float3 kD = aColor * LB_DirectionallightColor * nDotL;
    float3 kS = lerp(LB_DirectionallightColor, aColor, MB_Metalness) * pow(nDotH, MB_Shininess);
    //float3 kS = lerp(LB_DirectionallightColor, aColor, MB_Metalness) * pow(rDotv, MB_Shininess);
    return (kD + kS) * LB_DirectionallightIntensity;
}

float3 GetBlinnPhongLight(float3 aPosition, float3 aPixelNormal, float3 aColor)
{
    float3 result = 0;
    const float3 v = normalize(FB_CameraPosition - aPosition);
    
#ifndef _RETAIL
    if(FB_LightMode == 5)
    {
        result = aColor;
    }
    
    if(FB_LightMode == 0 || FB_LightMode == 1)
    {
#endif
    result = GetPhongAmbientlight(aPixelNormal, v, aColor);
#ifndef _RETAIL
    }
    if(FB_LightMode == 0 || FB_LightMode == 2)
    {
#endif
    result += GetPhongDirectionallight(aPosition, aPixelNormal, v, aColor);
    #ifndef _RETAIL
    }
#endif
    [unroll]
    for (int i = 0; i < MAX_LIGHTSOURCES; i++)
    {
#ifndef _RETAIL
    if(FB_LightMode == 0 || FB_LightMode == 3)
    {
#endif    
        result += GetPhongPointlightValue(aPosition, aPixelNormal, v, LB_Pointlights[i], aColor);
#ifndef _RETAIL
    }
    if(FB_LightMode == 0 || FB_LightMode == 4)
    {
#endif
        result += GetPhongSpotLightValue(aPosition, aPixelNormal, v, LB_Spotlights[i], aColor);
#ifndef _RETAIL
    }
#endif
    }

    return result;
}

#endif // LIGHTFUNCTIONS_HLSLI