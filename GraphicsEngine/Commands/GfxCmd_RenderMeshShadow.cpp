#include "GraphicsEngine.pch.h"
#include "GfxCmd_RenderMeshShadow.h"

GfxCmd_RenderMeshShadow::GfxCmd_RenderMeshShadow(const MeshComponent& aMesh) : myMeshElements(aMesh.GetElements()), myTransformMatrix(aMesh.GetTransform()), myWorldPosition(aMesh.GetWorldPosition()), myHasBones(false), myBoneTransforms()
#ifdef _DEBUG
, myMeshName(aMesh.GetName())
#endif // _DEBUG
{
}

GfxCmd_RenderMeshShadow::GfxCmd_RenderMeshShadow(const AnimatedMeshComponent& aMesh) : myMeshElements(aMesh.GetElements()), myTransformMatrix(aMesh.GetTransform()), myWorldPosition(aMesh.GetWorldPosition()), myHasBones(true), myBoneTransforms(aMesh.GetBoneTransforms())
#ifdef _DEBUG
, myMeshName(aMesh.GetName())
#endif // _DEBUG
{
}

void GfxCmd_RenderMeshShadow::Execute()
{
	SetObjectBuffer();
	for (auto& element : myMeshElements)
	{
		RHI::ConfigureInputAssembler(element.myData->myPrimitiveTopology, element.myData->myVertexBuffer, element.myData->myIndexBuffer, element.myData->myStride, Vertex::InputLayout);
		RHI::DrawIndexed(element.myData->myIndexCount);
	}
}

const CommonUtilities::Vector3f& GfxCmd_RenderMeshShadow::GetWorldPosition() const
{
	return myWorldPosition;
}

void GfxCmd_RenderMeshShadow::SetObjectBuffer()
{
	ObjectBuffer& buffer = GetObjectBuffer();
	buffer.Data.Transform = myTransformMatrix;
	buffer.Data.HasBones = myHasBones;
	if (myHasBones)
	{
		std::copy(myBoneTransforms.begin(), myBoneTransforms.end(), buffer.Data.BoneTransforms);
	}

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER, 1, buffer);
}
