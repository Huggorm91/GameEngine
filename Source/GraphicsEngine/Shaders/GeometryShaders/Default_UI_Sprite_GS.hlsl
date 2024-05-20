#include "../Default/ParticleData.hlsli"

[maxvertexcount(4)]
void main(
	point SpriteVertexToGeometry input[1],
	inout TriangleStream<SpriteGeometryToPixel> output
)
{
    const float2 offsets[4] =
    {
        float2(-1, 1),
        float2(1, 1),
        float2(-1, -1),
        float2(1, -1)
    };
    const float2 uv[4] =
    {
        float2(0, 0),
        float2(1, 0),
        float2(0, 1),
        float2(1, 1)
    };
    
    const SpriteVertexToGeometry inputData = input[0];
    
    SpriteGeometryToPixel result;
    result.Color = inputData.Color;
    result.Speed = inputData.Speed;
    result.LifeTime = inputData.LifeTime;
    result.Direction = inputData.Direction;
    
    for (uint i = 0; i < 4; i++)
    {
        result.Position = inputData.WorldPosition;
        result.Position.xy += offsets[i] * inputData.Scale.xy;
        
        result.UV = uv[i];
        output.Append(result);
    }
}