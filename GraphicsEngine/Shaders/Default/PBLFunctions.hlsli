#ifndef PBLFUNCTIONS_HLSLI
#define PBLFUNCTIONS_HLSLI
#include "DefaultMaterialData.hlsli"
static const float PI = 3.1415926535f;

// Functions required for convoluting a cubemap
float2 Hammersley(float i, float numSamples)
{
    uint b = uint(i);

    b = (b << 16u) | (b >> 16u);
    b = ((b & 0x55555555u) << 1u) | ((b & 0xAAAAAAAAu) >> 1u);
    b = ((b & 0x33333333u) << 2u) | ((b & 0xCCCCCCCCu) >> 2u);
    b = ((b & 0x0F0F0F0Fu) << 4u) | ((b & 0xF0F0F0F0u) >> 4u);
    b = ((b & 0x00FF00FFu) << 8u) | ((b & 0xFF00FF00u) >> 8u);

    const float radicalInverseVDC = float(b) * 2.3283064365386963e-10;
    return float2((i / numSamples), radicalInverseVDC);
}

float3 ImportanceSampleGGX(float2 aXi, float3 aNormal, float aRoughness)
{
    const float roughnesSq = aRoughness * aRoughness;

    const float phi = 2.0f * PI * aXi.x;
    const float cosTheta = sqrt((1.0f - aXi.y) / (1.0f + (roughnesSq * roughnesSq - 1) * aXi.y));
    const float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

    float3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    const float3 up = abs(aNormal.z) < 0.999 ? float3(0, 0, 1) : float3(1, 0, 0);
    const float3 tangent = normalize(cross(up, aNormal));
    const float3 bitangent = cross(aNormal, tangent);

    return normalize(tangent * H.x + bitangent * H.y + aNormal * H.z);
}

float GeometricAttenuation_Schlick_GGX_IBL(float aNdotV, float aRoughness)
{
    // Note different k here when calculating G GGX for IBL!
    const float a = aRoughness;
    const float k = (a * a) * 0.5f;

    const float nominator = aNdotV;
    const float denominator = aNdotV * (1.0 - k) + k;

    return nominator / denominator;
}

float GeometricAttenuation_Smith_IBL(float3 aN, float3 aV, float3 aL, float aRoughness)
{
    const float NdotV = saturate(dot(aN, aV));
    const float NdotL = saturate(dot(aN, aL));
    
    const float GGX_NdotV = GeometricAttenuation_Schlick_GGX_IBL(NdotV, aRoughness);
    const float GGX_NdotL = GeometricAttenuation_Schlick_GGX_IBL(NdotL, aRoughness);

    return GGX_NdotL * GGX_NdotV;
}

// LUT creation function for caculating a BRDF lookup for the second half of the split-sum equation.
// Run this once with float2 result = IntegrateBRDF(uv.x, uv.y); Causes a LUT to be generated.
float2 IntegrateBRDF(float aNdotV, float aRoughness)
{
    float3 V;
    V.x = sqrt(1.0f - aNdotV * aNdotV);
    V.y = 0.0;
    V.z = aNdotV;

    float a = 0;
    float b = 0;

    const float3 N = float3(0, 0, 1);

    const uint NUM_SAMPLES = 1024u;
    for (uint i = 0u; i < NUM_SAMPLES; ++i)
    {
        const float2 xi = Hammersley(i, NUM_SAMPLES);
        const float3 H = ImportanceSampleGGX(xi, N, aRoughness);
        const float3 L = normalize(2.0 * dot(V, H) * H - V);

        const float NdotL = saturate(L.z);
        const float NdotH = saturate(H.z);
        const float VdotH = saturate(dot(V, H));
        const float NdotV = saturate(dot(N, V));

        if (NdotL > 0.0)
        {
            const float G = GeometricAttenuation_Smith_IBL(N, V, L, aRoughness);
            const float G_Vis = (G * VdotH) / (NdotH * NdotV);
            const float Fc = pow(1.0 - VdotH, 5.0);

            a += (1.0 - Fc) * G_Vis;
            b += Fc * G_Vis;
        }
    }

    return float2(a, b) / NUM_SAMPLES;
}

float3 GetPblPointlightValue(float3 aPosition, float3 aPixelNormal, float3 aV, PointlightData aPointLight, float3 aColor)
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

float3 GetPblSpotLightValue(float3 aPosition, float3 aPixelNormal, float3 aV, SpotlightData aSpotLight, float3 aColor)
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

float3 GetPblDirectionallight(float3 aPosition, float3 aPixelNormal, float3 aV, float3 aColor)
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

float3 GetPblPointlights(float3 aPosition, float3 aPixelNormal, float3 aV, float3 aColor)
{
    float3 result = 0;
    [unroll]
    for (int i = 0; i < 8; i++)
    {
        [flatten]
        if (LB_Pointlights[i].Intensity > 0)
        {
            result += GetPblPointlightValue(aPosition, aPixelNormal, aV, LB_Pointlights[i], aColor);
        }
    }
    return result;
}

float3 GetPblSpotlights(float3 aPosition, float3 aPixelNormal, float3 aV, float3 aColor)
{
    float3 result = 0;
    [unroll]
    for (int i = 0; i < 8; i++)
    {
        [flatten]
        if (LB_Spotlights[i].Intensity > 0)
        {
            result += GetPblSpotLightValue(aPosition, aPixelNormal, aV, LB_Spotlights[i], aColor);
        }
    }
    return result;
}

float3 GetPblLight(float3 aPosition, float3 aPixelNormal, float3 aColor)
{
    float3 result = 0;
    const float3 v = normalize(FB_CameraPosition - aPosition);
    
#ifdef _DEBUG
    if(LB_LightMode == 5)
    {
        return aColor;
    }
    
    if(LB_LightMode == 0 || LB_LightMode == 1)
    {
#endif
    const float3 reflection = normalize(reflect(-v, aPixelNormal));

    const uint cubeMips = GetNumMips(EnvironmentCubeMap);
    float mipLvl = clamp(cubeMips * (1 - MB_Shininess * 0.001f), 0, cubeMips - 1);
    const float3 ambientKD = EnvironmentCubeMap.SampleLevel(DefaultSampler, aPixelNormal, mipLvl).rgb;
    const float3 ambientKS = EnvironmentCubeMap.SampleLevel(DefaultSampler, reflection, mipLvl).rgb;
    const float3 ambientLight = (ambientKD + ambientKS) * LB_AmbientlightIntensity;
    
    result = aColor * ambientLight;
#ifdef _DEBUG
    }
    if(LB_LightMode == 0 || LB_LightMode == 2)
    {
#endif
    result += GetPblDirectionallight(aPosition, aPixelNormal, v, aColor);
#ifdef _DEBUG
    }
    if(LB_LightMode == 0 || LB_LightMode == 3)
    {
#endif
    result += GetPblPointlights(aPosition, aPixelNormal, v, aColor);
#ifdef _DEBUG
    }
    if(LB_LightMode == 0 || LB_LightMode == 4)
    {
#endif
    result += GetPblSpotlights(aPosition, aPixelNormal, v, aColor);
#ifdef _DEBUG
    }
#endif
    return result;
}

#endif // PBLFUNCTIONS_HLSLI