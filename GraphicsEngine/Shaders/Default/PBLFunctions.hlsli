#ifndef PBLFUNCTIONS_HLSLI
#define PBLFUNCTIONS_HLSLI
#include "DefaultMaterialData.hlsli"
static const float PI = 3.1415926535f;
static const float PI_INVERSE = 0.31830988618379067153776752674503f; // 1/PI

struct LightData
{
    float3 albedoColor;
    float3 pixelNormal;
    float3 position;
    float3 v;
    float3 brdfDiffuse;
    float3 specularColor;
    float3 invertedSpecular;
    float occlusion;
    float roughness;
    float metalness;
    float k;
    float kInverse;
    float aPow2;
};

float3 CalculateSpecular(float aK, float aKInverse, float anAPow2, float aNDotL, float3 aNormal, float3 aV, float3 aInvertedLightDirection, float3 aSpecularColor, float3 aInvertedSpecular)
{
    const float3 LplusV = aInvertedLightDirection + aV;
    const float3 h = normalize(LplusV / length(LplusV));
    
    const float NdotH = saturate(dot(aNormal, h));
    const float D = anAPow2 / (PI * Pow2(Pow2(NdotH) * (anAPow2 - 1) + 1));
    
    const float VdotH = saturate(dot(aV, h));
    const float p = -5.55473f * VdotH - 6.98316f * VdotH;
    const float3 F = aSpecularColor + aInvertedSpecular * pow(2, p);
    
    const float NdotV = saturate(dot(aNormal, aV));
    const float G1v = NdotV / (NdotV * aKInverse + aK);
    const float G1l = aNDotL / (aNDotL * aKInverse + aK);
    const float G = G1l * G1v;
    
    return saturate((D * F * G) / (4 * aNDotL * NdotV));
}

float3 GetPblPointlightValue(LightData someData, PointlightData aPointLight)
{
    const float3 invertedDirection = normalize(aPointLight.Position - someData.position);
    const float nDotL = saturate(dot(someData.pixelNormal, invertedDirection));
    
    const float3 kS = CalculateSpecular(someData.k, someData.kInverse, someData.aPow2, nDotL, someData.pixelNormal, someData.v, invertedDirection, someData.specularColor, someData.invertedSpecular);
    const float3 kD = someData.brdfDiffuse * (1.f - kS);
    const float3 lightColor = aPointLight.Color * aPointLight.Intensity;
    const float attenuation = saturate(GetRangeAttenuation(distance(someData.position, aPointLight.Position), max(aPointLight.Radius, 0.00001f)));
    
    return saturate((kD + kS) * attenuation * lightColor * nDotL);
}

float3 GetPblSpotlightValue(LightData someData, SpotlightData aSpotLight)
{
    const float3 invertedDirection = normalize(aSpotLight.Position - someData.position);
    const float nDotL = saturate(dot(someData.pixelNormal, invertedDirection));
    
    const float3 kS = CalculateSpecular(someData.k, someData.kInverse, someData.aPow2, nDotL, someData.pixelNormal, someData.v, invertedDirection, someData.specularColor, someData.invertedSpecular);
    const float3 kD = someData.brdfDiffuse * (1.f - kS);
    const float3 lightColor = aSpotLight.Color * aSpotLight.Intensity;
    
    const float widthAttenuation = saturate((dot(-aSpotLight.LightDirection, invertedDirection) - sin(aSpotLight.OuterAngle)) / max(cos(aSpotLight.InnerAngle) - cos(aSpotLight.OuterAngle), 0.00001f));
    //const float widthAttenuation = pow(saturate((dot(-aSpotLight.LightDirection, invertedDirection) - sin(aSpotLight.OuterAngle)) / max(aSpotLight.ConeIntensityDifference, 0.00001f)), 2);
    const float attenuation = saturate(widthAttenuation * GetRangeAttenuation(distance(someData.position, aSpotLight.Position), max(aSpotLight.Range, 0.00001f)));
    
    return saturate((kD + kS) * attenuation * lightColor * nDotL);
}

float3 GetPblAmbientlight(LightData someData)
{
    //const float3 reflection = normalize(reflect(-someData.v, someData.pixelNormal));

    //const uint cubeMips = GetNumMips(EnvironmentCubeMap);
    //float mipLvl = clamp(cubeMips * (1 - MB_Shininess * 0.001f), 0, cubeMips - 1);
    //const float3 ambientKD = EnvironmentCubeMap.SampleLevel(DefaultSampler, someData.pixelNormal, cubeMips - 1).rgb;
    //const float3 ambientKS = EnvironmentCubeMap.SampleLevel(DefaultSampler, reflection, mipLvl).rgb;
    //const float3 ambientLight = (ambientKD + ambientKS) * LB_AmbientlightIntensity;
    
    //return someData.albedoColor * ambientLight;
    return 0;
}

float3 GetPblDirectionallight(LightData someData)
{
    const float nDotL = saturate(dot(someData.pixelNormal, LB_InvertedDirection));
    
    const float3 kS = CalculateSpecular(someData.k, someData.kInverse, someData.aPow2, nDotL, someData.pixelNormal, someData.v, LB_InvertedDirection, someData.specularColor, someData.invertedSpecular);
    const float3 kD = someData.brdfDiffuse * (1.f - kS);
    const float3 lightColor = LB_DirectionallightColor * LB_DirectionallightIntensity;
    
    return saturate((kD + kS) * lightColor * nDotL);
}

float3 GetPblLight(LightData someData)
{
    float3 result = 0;
    someData.v = normalize(FB_CameraPosition - someData.position);
    someData.brdfDiffuse = lerp((float3) 0.f, someData.albedoColor, 1 - someData.metalness) * PI_INVERSE;
    someData.specularColor = lerp((float3) 0.04f, someData.albedoColor, someData.metalness); 
    someData.invertedSpecular = 1 - someData.specularColor;
    const float a = Pow2(someData.roughness);
    someData.aPow2 = Pow2(a);
    someData.k = a * 0.125f; // a / 8
    someData.kInverse = 1 - someData.k;
    
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
#endif
    [unroll]
    for (int i = 0; i < 8; i++)
    {
#ifdef _DEBUG
    if(LB_LightMode == 0 || LB_LightMode == 3)
    {
#endif    
        result += GetPblPointlightValue(someData, LB_Pointlights[i]);    
#ifdef _DEBUG
    }
    if(LB_LightMode == 0 || LB_LightMode == 4)
    {
#endif
        result += GetPblSpotlightValue(someData, LB_Spotlights[i]);
#ifdef _DEBUG
    }
#endif
    }
    return result;
}

#endif // PBLFUNCTIONS_HLSLI