#ifndef PBLFUNCTIONS_HLSLI
#define PBLFUNCTIONS_HLSLI
#include "DefaultMaterialData.hlsli"
static const float PI = 3.1415926535f;

struct LightData
{
    float3 albedoColor;
    float3 pixelNormal;
    float3 position;
    float3 v;
    float occlusion;
    float roughness;
    float metalness;
};

float3 GetPblPointlightValue(LightData someData, PointlightData aPointLight)
{
    //const float3 invertedDirection = normalize(aPointLight.Position - aPosition);
    //const float nDotL = max(dot(aPixelNormal, invertedDirection), 0.f);

    //const float3 LplusV = invertedDirection + aV;
    //const float3 h = normalize(LplusV / length(LplusV));
    //const float nDotH = saturate(dot(aPixelNormal, h));
    
    //const float3 kD = aColor * aPointLight.Color * nDotL;
    //const float3 kS = lerp(aPointLight.Color, aColor, MB_Metalness) * pow(nDotH, MB_Shininess);
    //const float attenuation = saturate(GetRangeAttenuation(distance(aPosition, aPointLight.Position), max(aPointLight.Radius, 0.00001f)));
    
    //return (kD + kS) * attenuation * aPointLight.Intensity;
    return 0;
}

float3 GetPblSpotLightValue(LightData someData, SpotlightData aSpotLight)
{
    //const float3 invertedDirection = normalize(aSpotLight.Position - aPosition);
    //const float nDotL = max(dot(aPixelNormal, invertedDirection), 0.f);

    //const float3 LplusV = invertedDirection + aV;
    //const float3 h = normalize(LplusV / length(LplusV));
    //const float nDotH = saturate(dot(aPixelNormal, h));
    
    //const float3 kD = aColor * aSpotLight.Color * nDotL;
    //const float3 kS = lerp(aSpotLight.Color, aColor, MB_Metalness) * pow(nDotH, MB_Shininess);
    
    //const float widthAttenuation = pow(saturate((dot(-aSpotLight.LightDirection, invertedDirection) - sin(aSpotLight.OuterAngle)) / max(cos(aSpotLight.InnerAngle) - cos(aSpotLight.OuterAngle), 0.00001f)), 2);
    ////const float widthAttenuation = pow(saturate((dot(-aSpotLight.LightDirection, invertedDirection) - sin(aSpotLight.OuterAngle)) / max(aSpotLight.ConeIntensityDifference, 0.00001f)), 2);
    //const float attenuation = saturate(widthAttenuation * GetRangeAttenuation(distance(aPosition, aSpotLight.Position), max(aSpotLight.Range, 0.00001f)));
    
    //return (kD + kS) * attenuation * aSpotLight.Intensity;
    return 0;
}

float3 GetPblAmbientlight(LightData someData)
{
    const float3 reflection = normalize(reflect(-someData.v, someData.pixelNormal));

    const uint cubeMips = GetNumMips(EnvironmentCubeMap);
    float mipLvl = clamp(cubeMips * (1 - MB_Shininess * 0.001f), 0, cubeMips - 1);
    const float3 ambientKD = EnvironmentCubeMap.SampleLevel(DefaultSampler, someData.pixelNormal, cubeMips - 1).rgb;
    const float3 ambientKS = EnvironmentCubeMap.SampleLevel(DefaultSampler, reflection, mipLvl).rgb;
    const float3 ambientLight = (ambientKD + ambientKS) * LB_AmbientlightIntensity;
    
    return someData.albedoColor * ambientLight;
}

float3 GetPblDirectionallight(LightData someData)
{
    //const float nDotL = max(dot(aPixelNormal, LB_InvertedDirection), 0.f);

    //const float3 LplusV = LB_InvertedDirection + aV;
    //const float3 h = normalize(LplusV / length(LplusV));
    //const float nDotH = max(dot(aPixelNormal, h), 0.f);
    
    ////const float3 r = reflect(-LB_InvertedDirection, aPixelNormal);
    ////const float rDotv = saturate(dot(r, aV));
    
    //float3 kD = aColor * LB_DirectionallightColor * nDotL;
    //float3 kS = lerp(LB_DirectionallightColor, aColor, MB_Metalness) * pow(nDotH, MB_Shininess);
    ////float3 kS = lerp(LB_DirectionallightColor, aColor, MB_Metalness) * pow(rDotv, MB_Shininess);
    //return (kD + kS) * LB_DirectionallightIntensity;
    return 0;
}

float3 GetPblPointlights(LightData someData)
{
    float3 result = 0;
    [unroll]
    for (int i = 0; i < 8; i++)
    {
        [flatten]
        if (LB_Pointlights[i].Intensity > 0)
        {
            result += GetPblPointlightValue(someData, LB_Pointlights[i]);
        }
    }
    return result;
}

float3 GetPblSpotlights(LightData someData)
{
    float3 result = 0;
    [unroll]
    for (int i = 0; i < 8; i++)
    {
        [flatten]
        if (LB_Spotlights[i].Intensity > 0)
        {
            result += GetPblSpotLightValue(someData, LB_Spotlights[i]);
        }
    }
    return result;
}

float3 GetPblLight(LightData someData)
{
    float3 result = 0;
    someData.v = normalize(FB_CameraPosition - someData.position);
    
#ifdef _DEBUG
    if(LB_LightMode == 5)
    {
        result = someData.albedoColor;
    }
    
    if(LB_LightMode == 0 || LB_LightMode == 1)
    {
#endif    
    result = GetPblAmbientlight(someData);
#ifdef _DEBUG
    }
    if(LB_LightMode == 0 || LB_LightMode == 2)
    {
#endif
    result += GetPblDirectionallight(someData);
#ifdef _DEBUG
    }
    if(LB_LightMode == 0 || LB_LightMode == 3)
    {
#endif
    result += GetPblPointlights(someData);
#ifdef _DEBUG
    }
    if(LB_LightMode == 0 || LB_LightMode == 4)
    {
#endif
    result += GetPblSpotlights(someData);
#ifdef _DEBUG
    }
#endif
    return result;
}

#endif // PBLFUNCTIONS_HLSLI