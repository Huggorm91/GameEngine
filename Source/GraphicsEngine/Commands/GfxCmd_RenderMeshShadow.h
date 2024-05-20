#pragma once
#include "GraphicsCommand.h"
#include "AssetManager/Assets/Components/Rendering/AnimatedMeshComponent.h"

class GfxCmd_RenderMeshShadow : public GraphicsCommand
{
public:
	GfxCmd_RenderMeshShadow(const MeshComponent& aMesh);
	GfxCmd_RenderMeshShadow(const AnimatedMeshComponent& aMesh);
	~GfxCmd_RenderMeshShadow() override = default;

	void Execute() override;

	const Crimson::Vector3f& GetWorldPosition() const;

private:
	std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT> myBoneTransforms;
	std::vector<MeshElement> myMeshElements;
	Crimson::Matrix4x4f myTransformMatrix;
	Crimson::Vector3f myWorldPosition;
	bool myHasBones;
#ifdef _DEBUG
	std::string myMeshName;
#endif // _DEBUG

	void SetObjectBuffer();
};