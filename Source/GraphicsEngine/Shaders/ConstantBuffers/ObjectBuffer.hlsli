#ifndef OBJECTBUFFER_HLSLI
#define OBJECTBUFFER_HLSLI

#define MAX_BONE_COUNT 256
cbuffer ObjectBuffer : register(b1)
{
    float4x4 OB_BoneTransforms[MAX_BONE_COUNT];
	float4x4 OB_Transform;
    float4x4 OB_TransformInverse;
	float4 OB_Color;
    uint OB_ID;
    bool OB_HasBones;
    float2 padding;
}
#endif // OBJECTBUFFER_HLSLI