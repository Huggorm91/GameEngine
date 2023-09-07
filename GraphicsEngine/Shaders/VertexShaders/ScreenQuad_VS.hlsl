#include "../Default/QuadData.hlsli"
QuadVSToPS main(unsigned int aVertexIndex : SV_VertexID)
{
    const float4 position[4] =
    {
        float4(-1, -1, 0, 1),
        float4(-1, 1, 0, 1),
        float4(1, -1, 0, 1),
        float4(1, 1, 0, 1)
    };
    const float2 uv[4] =
    {
        float2(0, 1),
        float2(0, 0),
        float2(1, 1),
        float2(1, 0)
    };
    
    QuadVSToPS output;
    output.Position = position[aVertexIndex];
    output.UV = uv[aVertexIndex];
    return output;
}