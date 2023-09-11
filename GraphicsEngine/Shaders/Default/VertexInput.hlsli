#ifndef DEFAULTVERTEXINPUT_HLSLI
#define DEFAULTVERTEXINPUT_HLSLI

struct DefaultVertexInput
{
    float2 UV0 : UV0;
    float2 UV1 : UV1;
    float2 UV2 : UV2;
    float2 UV3 : UV3;

    float4 Color0 : COLOR0;
    float4 Color1 : COLOR1;
    float4 Color2 : COLOR2;
    float4 Color3 : COLOR3;

    float4 Position : POSITION;

    uint4 BoneIDs : BONEIDS;
    float4 BoneWeights : BONEWEIGHTS;

    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

#endif // DEFAULTVERTEXINPUT_HLSLI