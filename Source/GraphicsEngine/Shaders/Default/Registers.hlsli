#ifndef REGISTERS_HLSLI
#define REGISTERS_HLSLI

static const int MAX_LIGHTSOURCES = 8; // Update registers for ShadowMaps if MAX_LIGHTSOURCES is changed

SamplerState DefaultSampler : register(s0);
SamplerState BlurSampler : register(s13);
SamplerComparisonState ShadowSampler : register(s14);
SamplerState LUTSampler : register(s15);

Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D MaterialTexture : register(t2);
Texture2D FXTexture : register(t3);

Texture2D SpotShadowMap[MAX_LIGHTSOURCES] : register(t86);
Texture2D DirectionalShadowMap : register(t94);

Texture2D LUTTexture : register(t95);
Texture2D DefaultFXTexture : register(t96);
Texture2D DefaultMaterialTexture : register(t97);
Texture2D DefaultNormalTexture : register(t98);
Texture2D MissingTexture : register(t99);

TextureCube EnvironmentCubeMap : register(t100);
TextureCube PointShadowMap[MAX_LIGHTSOURCES] : register(t120);

#endif // REGISTERS_HLSLI