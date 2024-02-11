#include "../Default/ParticleData.hlsli"

SpritePixelOutput main(SpriteGeometryToPixel input)
{
    SpritePixelOutput result;
    float4 textureColor = AlbedoTexture.Sample(DefaultSampler, input.UV);
    
    if (textureColor.a <= 0.05f)
    {
        discard;
        return result;
    }
    
    const float4 lerpColor = lerp(PB_StartColor, PB_EndColor, float4(input.LerpValue));
    result.Color = textureColor * input.Color;
    result.Color *= lerpColor;
    return result;
}