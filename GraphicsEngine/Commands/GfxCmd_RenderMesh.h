#pragma once
#include "GraphicsCommand.h"
#include "AssetManager/Assets/Components/Rendering/AnimatedMeshComponent.h"

class GfxCmd_RenderMesh : public GraphicsCommand
{
public:
	GfxCmd_RenderMesh(const MeshComponent& aMesh);
	GfxCmd_RenderMesh(const AnimatedMeshComponent& aMesh);
	~GfxCmd_RenderMesh() override = default;

	void Execute() override;

	const CommonUtilities::Vector3f& GetWorldPosition() const;

private:
	std::array<CommonUtilities::Matrix4x4f, 128> myBoneTransforms;
	std::vector<MeshElement> myMeshElements;
	CommonUtilities::Matrix4x4f myTransformMatrix;
	CommonUtilities::Vector4f myWorldPosition;
	CommonUtilities::Vector4f myColor;
	bool myHasBones;
#ifdef _DEBUG
	std::string myMeshName;
#endif // _DEBUG

	void SetObjectBuffer();
	void SetMaterialResource(const Material& aMaterial);
};