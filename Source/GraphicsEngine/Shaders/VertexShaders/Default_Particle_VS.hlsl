#include "../Default/ParticleData.hlsli"

SpriteVertexToGeometry main(SpriteVertexInput input)
{
    SpriteVertexToGeometry result;
    result.WorldPosition = mul(PB_Transform, input.Position);
    result.Color = input.Color;
    result.Scale = input.Scale;
    result.Velocity = input.Velocity;
    result.LifeTime = input.LifeTime;
    return result;
}