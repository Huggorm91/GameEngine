#include "../Default/ParticleData.hlsli"

SpriteVertexToGeometry main(SpriteVertexInput input)
{
    SpriteVertexToGeometry result;
    result.WorldPosition.xyz = 2 * input.Position.xyz - 1; // TODO: change this form 0-1 values to pixel coords
    result.WorldPosition.w = input.Position.w;
    
    result.Color = input.Color;
    result.Scale = input.Scale; // TODO: convert this from pixel size to screenssize
    result.Speed = input.Speed;
    result.LifeTime = input.LifeTime;
    result.Direction = input.Direction;
    return result;
}