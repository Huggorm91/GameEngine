#ifndef OBJECTBUFFER_HLSLI
#define OBJECTBUFFER_HLSLI
cbuffer ObjectBuffer : register(b1)
{
    float4x4 OB_BoneTransforms[128];
	float4x4 OB_Transform;
	float4 OB_Color;
    bool OB_HasBones;
    float3 padding;
}
#endif // OBJECTBUFFER_HLSLI