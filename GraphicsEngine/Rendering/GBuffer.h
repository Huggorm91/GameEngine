#pragma once
#include "Vector4.hpp"
#include "Texture.h"

class GBuffer
{
public:
	GBuffer() = default;
	~GBuffer() = default;

	bool Init();

	void SetAsTarget(const Texture* aDepthStencil);
	void SetAsResource(unsigned aStage, unsigned aSlot);
	void UnbindAsResource(unsigned aStage, unsigned aSlot);

	void ClearTextures();

private:
	Texture myAlbedoMap;
	Texture myMaterialMap;
	Texture myVertexNormalMap;
	Texture myPixelNormalMap;
	Texture myPositionMap;
	Texture myPickingMap;
	std::vector<Texture*> myTextureList;
	std::vector<Texture*> myNullPtrList;

	bool CreateAlbedoTexture(const RHI::DeviceSize& aSize);
	bool CreateMaterialTexture(const RHI::DeviceSize& aSize);
	bool CreateVertexNormalTexture(const RHI::DeviceSize& aSize);
	bool CreatePixelNormalTexture(const RHI::DeviceSize& aSize);
	bool CreatePositionTexture(const RHI::DeviceSize& aSize);
	bool CreatePickingTexture(const RHI::DeviceSize& aSize);
};