#pragma once
#include "GraphicsCommand.h"
#include "AssetManager/Assets/Components/Rendering/AnimatedMeshComponent.h"

class GfxCmd_SetObjectBuffer : public GraphicsCommand
{
public:
	GfxCmd_SetObjectBuffer(const MeshComponent& aMesh);
	GfxCmd_SetObjectBuffer(const AnimatedMeshComponent& aMesh);

	void Execute() override;

private:
	std::array<CommonUtilities::Matrix4x4f, 128> myBoneTransforms;
	CommonUtilities::Matrix4x4f myTransformMatrix;
	CommonUtilities::Vector4f myColor;
	bool myHasBones;
};