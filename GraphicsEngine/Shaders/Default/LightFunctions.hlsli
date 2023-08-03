#ifndef LIGHTFUNCTIONS_HLSLI
#define LIGHTFUNCTIONS_HLSLI
#include "../ConstantBuffers/LightBuffer.hlsli"

//float3 GetPointlightValue(float4 aPosition, float3 aPixelNormal, PointLightData aPointLight)
//{
//    float3 inverseLightDirection = normalize(aPointLight.Position - aPosition);
//   // const float3 cubeMap = environmentCube.Sample(defaultSampler, inverseLightDirection);
    
//    float3 kD = aPointLight.DiffuseColor;
//    float nL = dot(aPixelNormal, inverseLightDirection);

//    kD = saturate(kD * nL);
//    kD.a = 1;
    
//    float3 kS = aPointLight.SpecularColor; //Specular color
//    float3 v = normalize(FB_Pos - aPosition);
//    float3 h = ((inverseLightDirection) + v) / length((inverseLightDirection) + v);
//    float nDotH = saturate(dot(aPixelNormal, h));
//    kS = kS * (pow(nDotH, Material.Shininess) * aPointLight.Intensity);
//    kS.a = 1;
    
//    float aPL; //Light attenuation
//    float d = distance(aPosition, aPointLight.Position); //Distance between the light and the pixel
//    float lightRange = max(aPointLight.Range, 0.00001f);
//    aPL = saturate(1 - pow(d * (1 / lightRange), 2));
    
//    return (kD + kS) * aPL * aPointLight.Intensity;
//}

//float3 GetSpotLightValue(float4 aPosition, float3 aPixelNormal, SpotLightData aSpotLight)
//{
//    //const float3 cubeMap = environmentCube.Sample(defaultSampler, aSpotLight.Direction);
//    float3 inverseLightDirection = normalize(aSpotLight.Position - aPosition);
    
//    float3 kD = aSpotLight.DiffuseColor;
//    float nL = dot(aPixelNormal, inverseLightDirection);

//    kD = saturate(kD * nL);
//    kD.a = 1;
    
//    float3 kS = aSpotLight.SpecularColor; //Specular color
//    float3 v = normalize(FB_Pos - aPosition);
//    float3 h = ((inverseLightDirection) + v) / length((inverseLightDirection) + v);
//    float nDotH = saturate(dot(aPixelNormal, h));
//    kS = kS * (pow(nDotH, Material.Shininess) * aSpotLight.Intensity);
//    kS.a = 1;
    
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
//}

//float3 GetDirectionallight(float4 aPosition, float3 aPixelNormal)
//{
//    float3 inverseLightDirection = normalize(-DirectionalLight.Direction);
    
//    //const float3 cubeMap = environmentCube.Sample(defaultSampler, DirectionalLight.Direction);
    
//    float3 kD = DirectionalLight.DiffuseColor;
//    float nL = dot(aPixelNormal, inverseLightDirection);

//    kD = saturate(kD * nL);
//    kD.a = 1;
    
//    const int cubeMips = GetNumMips(environmentCube);
    
//    float3 kS = DirectionalLight.SpecularColor; //Specular color
//    float3 v = normalize(FB_Pos - aPosition);
    
    
//    float3 h = ((inverseLightDirection) + v) / length((inverseLightDirection) + v);
//    float nDotH = saturate(dot(aPixelNormal, h));
//    kS = kS * (pow(nDotH, Material.Shininess) * DirectionalLight.Intensity);
//    kS.a = 1;
//    kS = AvarageColor(kS);
//    return (kD + kS) * DirectionalLight.Intensity;
//}

//float3 GetPointlight(float4 aPosition, float3 aPixelNormal)
//{
//    float3 result = 0;
//    [unroll]
//    for (int i = 0; i < 8; i++)
//    {
//        [flatten]
//        if (PointLights[i].Intensity > 0)
//        {
//            result += CalculatePointLight(aPosition, aPixelNormal, PointLights[i]);
//        }
//    }
//    return result;
//}

//float3 GetSpotlight(float4 aPosition, float3 aPixelNormal)
//{
//    float3 result = 0;
//    [unroll]
//    for (int i = 0; i < 8; i++)
//    {
//        [flatten]
//        if (SpotLights[i].Intensity > 0)
//        {
//            result += CalculateSpotLight(aPosition, aPixelNormal, SpotLights[i]);
//        }
//    }
//    return result;
//}

#endif // LIGHTFUNCTIONS_HLSLI