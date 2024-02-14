#include "../Default/ParticleData.hlsli"

SpriteVertexToGeometry main(SpriteVertexInput input)
{
    SpriteVertexToGeometry result;
    result.WorldPosition = mul(PB_Transform, input.Position);
    result.Color = input.Color;
    result.Scale = input.Scale;
    result.Speed = input.Speed;
    result.LifeTime = input.LifeTime;
    result.Direction = input.Direction;
    return result;
}