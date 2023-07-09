#include "Default/DefaultMaterialData.hlsli"

DefaultVertexToPixel main(DefaultVertexInput input)
{
	DefaultVertexToPixel result;

	result.Color = GetAlphaBlendColor(OB_Color, input.Color0);
	result.Color = GetAlphaBlendColor(result.Color, input.Color1);
	result.Color = GetAlphaBlendColor(result.Color, input.Color2);
	result.Color = GetAlphaBlendColor(result.Color, input.Color3);

	result.UVs[0] = input.UV0;
	result.UVs[1] = input.UV1;
	result.UVs[2] = input.UV2;
	result.UVs[3] = input.UV3;

    result.NormalOS = normalize(input.Normal);
    result.TangentOS = normalize(input.Tangent);
    result.BinormalOS = normalize(cross(input.Normal, input.Tangent));

	const float3x3 transform3x3 = (float3x3)OB_Transform;

	result.NormalWS = normalize(mul(transform3x3, result.NormalOS));
	result.TangentWS = normalize(mul(transform3x3, result.TangentOS));
	result.BinormalWS = normalize(mul(transform3x3, result.BinormalOS));

	result.LocalPosition = input.Position;
	result.Position = input.Position;
	
	[flatten]
	if (OB_HasBones)
	{
		const float4x4 skinMatrix = GetSkinMatrix(input.BoneWeights, input.BoneIDs, OB_BoneTransforms);
		result.Position = mul(skinMatrix, result.Position);
		result.LocalPosition = mul(skinMatrix, result.LocalPosition);

		const float3x3 skin3x3 = (float3x3)skinMatrix;
        result.NormalOS = normalize(mul(skin3x3, result.NormalOS));
        result.TangentOS = normalize(mul(skin3x3, result.TangentOS));
        result.BinormalOS = normalize(mul(skin3x3, result.BinormalOS));
        result.NormalWS = normalize(mul(skin3x3, result.NormalWS));
        result.TangentWS = normalize(mul(skin3x3, result.TangentWS));
        result.BinormalWS = normalize(mul(skin3x3, result.BinormalWS));
    }
	
	result.Position = mul(OB_Transform, result.Position);
	result.Position = mul(FB_View, result.Position);
	result.Position = mul(FB_Projection, result.Position);

	return result;
}