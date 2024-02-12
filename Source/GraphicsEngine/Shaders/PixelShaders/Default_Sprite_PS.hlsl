#include "../Default/ParticleData.hlsli"

SpritePixelOutput main(SpriteGeometryToPixel input)
{
    SpritePixelOutput result;
    float4 textureColor = AlbedoTexture.Sample(DefaultSampler, input.UV);
    
    if (textureColor.a <= 0.05f)
    {
        discard;
        result.Color = 0;
        return result;
    }
    
    result.Color = textureColor * input.Color;
    return result;
}