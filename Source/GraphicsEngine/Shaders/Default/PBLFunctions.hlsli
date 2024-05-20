#ifndef PBLFUNCTIONS_HLSLI
#define PBLFUNCTIONS_HLSLI
#include "MaterialData.hlsli"

struct LightData
{
    float4 position;
    float3 pixelNormal;
    float3 v;
    float3 brdfDiffuse;
    float3 specularColor;
    float3 invertedSpecular;
    float roughness;
    float metalness;
    float k;
    float kInverse;
    float aPow2;
    float nDotV;
};

float3 GetShadowMapUV(float4x4 aViewMatrix, float4x4 aProjectionMatrix, float4 aWorldPosition)
{
    float4 position = mul(aViewMatrix, aWorldPosition);
    position = mul(aProjectionMatrix, position);
    float3 uv = position.xyz / position.w;
    uv.xy = uv.xy * 0.5f + 0.5f;
    
    uv.y = 1 - uv.y;
    return uv;
}

float3 CalculateSpecular(float aK, float aKInverse, float anAPow2, float aNDotL, float aNDotV, float3 aNormal, float3 aV, float3 aInvertedLightDirection, float3 aSpecularColor, float3 aInvertedSpecular)
{
    const float3 LplusV = aInvertedLightDirection + aV;
    const float3 h = normalize(LplusV / length(LplusV));
    
    const float nDotH = saturate(dot(aNormal, h));
    const float D = anAPow2 / (PI * Pow2(Pow2(nDotH) * (anAPow2 - 1) + 1));
    
    const float vDotH = saturate(dot(aV, h));
    const float p = -5.55473f * vDotH - 6.98316f * vDotH;
    const float3 F = aSpecularColor + aInvertedSpecular * pow(2, p);

    const float G1v = aNDotV / (aNDotV * aKInverse + aK);
    const float G1l = aNDotL / (aNDotL * aKInverse + aK);
    const float G = G1l * G1v;
    
    return saturate((D * F * G) / (4 * aNDotL * aNDotV));
}

float3 GetPblPointlightValue(LightData someData, PointlightData aPointLight)
{
    const float3 invertedDirection = normalize(aPointLight.Position - (float3) someData.position);
    const float nDotL = saturate(dot(someData.pixelNormal, invertedDirection));
    
    const float3 kS = CalculateSpecular(someData.k, someData.kInverse, someData.aPow2, nDotL, someData.nDotV, someData.pixelNormal, someData.v, invertedDirection, someData.specularColor, someData.invertedSpecular);
    const float3 kD = someData.brdfDiffuse * (1.f - kS);
    const float3 lightColor = aPointLight.Color * aPointLight.Intensity;
    const float attenuation = saturate(GetRangeAttenuation(distance((float3) someData.position, aPointLight.Position), max(aPointLight.Radius, 0.00001f)));
    
    return (kD + kS) * attenuation * lightColor * nDotL;
}

float3 GetPblSpotlightValue(LightData someData, SpotlightData aSpotLight)
{
    const float3 invertedDirection = normalize(aSpotLight.Position - (float3) someData.position);
    const float nDotL = saturate(dot(someData.pixelNormal, invertedDirection));
    
    const float3 kS = CalculateSpecular(someData.k, someData.kInverse, someData.aPow2, nDotL, someData.nDotV, someData.pixelNormal, someData.v, invertedDirection, someData.specularColor, someData.invertedSpecular);
    const float3 kD = someData.brdfDiffuse * (1.f - kS);
    
    float shadowMultiplier = 1.f;
    //if (LB_CastDirectionalShadows)
    //{
    //    const float3 uv = GetShadowMapUV(aSpotLight.View, aSpotLight.Projection, someData.position);
    //    const float depth = uv.z - LB_ShadowBias;
    //    shadowMultiplier = DirectionalShadowMap.SampleCmpLevelZero(ShadowSampler, uv.xy, depth).r;
    //}
    const float3 lightColor = aSpotLight.Color * aSpotLight.Intensity * shadowMultiplier;
    
    const float widthAttenuation = saturate((dot(-aSpotLight.LightDirection, invertedDirection) - sin(aSpotLight.OuterAngle)) / max(cos(aSpotLight.InnerAngle) - cos(aSpotLight.OuterAngle), 0.00001f));
    const float attenuation = saturate(widthAttenuation * GetRangeAttenuation(distance((float3) someData.position, aSpotLight.Position), max(aSpotLight.Range, 0.00001f)));
    
    return (kD + kS) * attenuation * lightColor * nDotL;
}

float3 GetPblAmbientlight(LightData someData, float anOcclusion, float3 aDiffuseColor)
{
    const uint cubeMips = GetNumMips(EnvironmentCubeMap) -1;
    
    const float3 reflection = reflect(-someData.v, someData.pixelNormal);
    const float3 cubeMap = EnvironmentCubeMap.SampleLevel(DefaultSampler, reflection, cubeMips * someData.roughness).rgb;
    const float2 lut = LUTTexture.Sample(LUTSampler, float2(someData.nDotV, someData.roughness)).rg;

    const float3 kS = cubeMap * (someData.specularColor * lut.x + lut.y);
    const float3 kD = aDiffuseColor * EnvironmentCubeMap.SampleLevel(DefaultSampler, someData.pixelNormal, cubeMips).rgb;
    
    return (kD + kS) * anOcclusion * LB_AmbientlightIntensity;
}

float3 GetPblDirectionallight(LightData someData)
{
    const float nDotL = saturate(dot(someData.pixelNormal, LB_InvertedDirection));
    
    const float3 kS = CalculateSpecular(someData.k, someData.kInverse, someData.aPow2, nDotL, someData.nDotV, someData.pixelNormal, someData.v, LB_InvertedDirection, someData.specularColor, someData.invertedSpecular);
    const float3 kD = someData.brdfDiffuse * (1.f - kS);
    
    float shadowMultiplier = 1.f;
    if (LB_CastDirectionalShadows)
    {
        const float3 uv = GetShadowMapUV(LB_DirectionalView, LB_DirectionalProjection, someData.position);
        const float depth = uv.z - LB_ShadowBias;
        shadowMultiplier = DirectionalShadowMap.SampleCmpLevelZero(ShadowSampler, uv.xy, depth).r;
    }
    const float3 lightColor = LB_DirectionallightColor * LB_DirectionallightIntensity * shadowMultiplier;
    
    return (kD + kS) * lightColor * nDotL;
}

float3 GetPblLight(LightData someData, float3 anAlbedoColor, float anOcclusion)
{
    float3 result = 0;
    const float3 diffuseColor = lerp((float3) 0.f, anAlbedoColor, 1 - someData.metalness);
    someData.v = normalize(FB_CameraPosition - (float3) someData.position);
    someData.brdfDiffuse = diffuseColor * PI_INVERSE;
    someData.specularColor = lerp((float3) 0.04f, anAlbedoColor, someData.metalness);
    someData.invertedSpecular = 1 - someData.specularColor;
    const float a = Pow2(someData.roughness);
    someData.aPow2 = Pow2(a);
    someData.k = a * 0.125f; // a / 8
    someData.kInverse = 1 - someData.k;
    someData.nDotV = saturate(dot(someData.pixelNormal, someData.v));
    
#ifndef _RETAIL
    if(FB_LightMode == 5)
    {
        result = anAlbedoColor;
    }
    
    if (FB_LightMode == 0 || FB_LightMode == 1)
    {
#endif // !_RETAIL
    result = GetPblAmbientlight(someData, anOcclusion, diffuseColor);
#ifndef _RETAIL
    }
    if (FB_LightMode == 0 || FB_LightMode == 2)
    {
#endif // !_RETAIL
    result += GetPblDirectionallight(someData);
#ifndef _RETAIL
    }
#endif // !_RETAIL
    [unroll]
    for (int i = 0; i < MAX_LIGHTSOURCES; i++)
    {
#ifndef _RETAIL
    if(FB_LightMode == 0 || FB_LightMode == 3)
    {
#endif // !_RETAIL
        result += GetPblPointlightValue(someData, LB_Pointlights[i]);    
#ifndef _RETAIL
        }
    if(FB_LightMode == 0 || FB_LightMode == 4)
    {
#endif // !_RETAIL
        result += GetPblSpotlightValue(someData, LB_Spotlights[i]);
#ifndef _RETAIL
        }
#endif // !_RETAIL
    }
    return result;
}

#endif // PBLFUNCTIONS_HLSLI