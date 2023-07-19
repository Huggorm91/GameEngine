#ifndef LIGHTFUNCTIONS_HLSLI
#define LIGHTFUNCTIONS_HLSLI
#include "../ConstantBuffers/LightBuffer.hlsli"

//float3 GetPointlightValue(float4 aPixelPosition, float3 aPixelNormal, PointLightData aPointLight)
//{
//    float3 inverseLightDirection = normalize(aPointLight.Position - aPixelPosition);
//   // const float3 cubeMap = environmentCube.Sample(defaultSampler, inverseLightDirection.rgb);
    
//    float3 kD = aPointLight.DiffuseColor;
//    float nL = dot(aPixelNormal.rgb, inverseLightDirection.rgb);

//    kD.rgb = saturate(kD.rgb * nL);
//    kD.a = 1;
    
//    float3 kS = aPointLight.SpecularColor; //Specular color
//    float4 v = normalize(FB_Pos - aPixelPosition);
//    float3 h = ((inverseLightDirection.rgb) + v.rgb) / length((inverseLightDirection.rgb) + v.rgb);
//    float nDotH = saturate(dot(aPixelNormal, h));
//    kS.rgb = kS.rgb * (pow(nDotH, Material.Shininess) * aPointLight.Intensity);
//    kS.a = 1;
    
//    float aPL; //Light attenuation
//    float d = distance(aPixelPosition, aPointLight.Position); //Distance between the light and the pixel
//    float lightRange = max(aPointLight.Range, 0.00001f);
//    aPL = saturate(1 - pow(d * (1 / lightRange), 2));
    
//    return (kD + kS) * aPL * aPointLight.Intensity;
//}

//float3 GetSpotLightValue(float4 aPixelPosition, float3 aPixelNormal, SpotLightData aSpotLight)
//{
//    //const float3 cubeMap = environmentCube.Sample(defaultSampler, aSpotLight.Direction.rgb);
//    float3 inverseLightDirection = normalize(aSpotLight.Position - aPixelPosition);
    
//    float3 kD = aSpotLight.DiffuseColor;
//    float nL = dot(aPixelNormal.rgb, inverseLightDirection.rgb);

//    kD.rgb = saturate(kD.rgb * nL);
//    kD.a = 1;
    
//    float3 kS = aSpotLight.SpecularColor; //Specular color
//    float4 v = normalize(FB_Pos - aPixelPosition);
//    float3 h = ((inverseLightDirection.rgb) + v.rgb) / length((inverseLightDirection.rgb) + v.rgb);
//    float nDotH = saturate(dot(aPixelNormal, h));
//    kS.rgb = kS.rgb * (pow(nDotH, Material.Shininess) * aSpotLight.Intensity);
//    kS.a = 1;
    
//    float aLR; //Light range attenuation
//    float d = distance(aPixelPosition, aSpotLight.Position); //Distance between the light and the pixel
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

//float3 GetDirectionallight(float4 aPixelPosition, float3 aPixelNormal)
//{
//    float3 inverseLightDirection = normalize(-DirectionalLight.Direction);
    
//    //const float3 cubeMap = environmentCube.Sample(defaultSampler, DirectionalLight.Direction.rgb);
    
//    float3 kD = DirectionalLight.DiffuseColor;
//    float nL = dot(aPixelNormal.rgb, inverseLightDirection.rgb);

//    kD.rgb = saturate(kD.rgb * nL);
//    kD.a = 1;
    
//    const int cubeMips = GetNumMips(environmentCube);
    
//    float3 kS = DirectionalLight.SpecularColor; //Specular color
//    float4 v = normalize(FB_Pos - aPixelPosition);
    
    
//    float3 h = ((inverseLightDirection.rgb) + v.rgb) / length((inverseLightDirection.rgb) + v.rgb);
//    float nDotH = saturate(dot(aPixelNormal, h));
//    kS.rgb = kS.rgb * (pow(nDotH, Material.Shininess) * DirectionalLight.Intensity);
//    kS.a = 1;
//    kS = AvarageColor(kS);
//    return (kD + kS) * DirectionalLight.Intensity;
//}

//float3 GetPointlight(float4 aPixelPosition, float3 aPixelNormal)
//{
//    float3 result = 0;
//    [unroll]
//    for (uint p = 0; p < 8; p++)
//    {
//        [flatten]
//        if (PointLights[p].Intensity > 0)
//        {
//            result += CalculatePointLight(aPixelPosition, aPixelNormal, PointLights[p]);
//        }
//    }
//    return result;
//}

//float3 GetSpotlight(float4 aPixelPosition, float3 aPixelNormal)
//{
//    float3 result = 0;
//    [unroll]
//    for (uint s = 0; s < 8; s++)
//    {
//        [flatten]
//        if (SpotLights[s].Intensity > 0)
//        {
//            result += CalculateSpotLight(aPixelPosition, aPixelNormal, SpotLights[s]);
//        }
//    }
//    return result;
//}

#endif // LIGHTFUNCTIONS_HLSLI