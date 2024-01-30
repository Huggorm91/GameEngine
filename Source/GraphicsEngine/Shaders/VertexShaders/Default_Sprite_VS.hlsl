#include "../Default/ParticleData.hlsli"

SpriteVertexToGeometry main(SpriteVertexInput input)
{
    SpriteVertexToGeometry result;
    // TODO: Check if needed to multiply with object transform to move into worldspace
    //result.WorldPosition = mul(FB_View, input.Position); // This is done inside geometry shader
    result.WorldPosition = input.Position;
    result.Color = input.Color;
    result.Scale = input.Scale;
    result.Velocity = input.Velocity;
    result.LifeTime = input.LifeTime;
    return result;
}