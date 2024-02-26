#pragma once
#include "GraphicsCommand.h"
#include "AssetManager/Assets/Components/Rendering/AnimatedMeshComponent.h"

class GfxCmd_RenderMesh : public GraphicsCommand
{
public:
	GfxCmd_RenderMesh(const MeshComponent& aMesh, bool aIsDeferred);
	GfxCmd_RenderMesh(const AnimatedMeshComponent& aMesh, bool aIsDeferred);
	~GfxCmd_RenderMesh() override = default;

	void Execute() override;

	const Crimson::Vector4f& GetWorldPosition() const;

private:
	std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT> myBoneTransforms;
	std::vector<MeshElement> myMeshElements;
	Crimson::Matrix4x4f myTransformMatrix;
	Crimson::Vector4f myWorldPosition;
	Crimson::Vector4f myColor;
	unsigned myID;
	bool myHasBones;
#ifdef _DEBUG
	std::string myMeshName;
#endif // _DEBUG

	void SetObjectBuffer();
	void SetMaterialResource(const Material& aMaterial);
};