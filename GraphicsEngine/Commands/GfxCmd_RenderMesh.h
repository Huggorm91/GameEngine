#pragma once
#include "GraphicsCommand.h"
#include "AssetManager/Assets/Components/Rendering/MeshComponent.h"

class GfxCmd_RenderMesh : public GraphicsCommand
{
public:
	GfxCmd_RenderMesh(const MeshComponent& aMesh);

	void Execute() override;

private:
	std::vector<MeshElement> myMeshElements;
#ifdef _DEBUG
	std::string myMeshName;
#endif // _DEBUG


	void SetMaterialResource(const Material& aMaterial);
};