#include "GraphicsEngine.pch.h"
#include "GBuffer.h"

unsigned GBuffer::mySlot;

bool GBuffer::Init(unsigned aSlot)
{
	mySlot = aSlot;

	const RHI::DeviceSize& size = RHI::GetDeviceSize();
	if (!CreateAlbedoTexture(size))
	{
		GELogger.Err("GBuffer: Failed to create Albedo texture");
		return false;
	}
	if (!CreateMaterialTexture(size))
	{
		GELogger.Err("GBuffer: Failed to create Material texture");
		return false;
	}
	if (!CreateVertexNormalTexture(size))
	{
		GELogger.Err("GBuffer: Failed to create Vertex Normal texture");
		return false;
	}
	if (!CreatePixelNormalTexture(size))
	{
		GELogger.Err("GBuffer: Failed to create Pixel Normal texture");
		return false;
	}
	if (!CreatePositionTexture(size))
	{
		GELogger.Err("GBuffer: Failed to create Position texture");
		return false;
	}
	if (!CreateFXTexture(size))
	{
		GELogger.Err("GBuffer: Failed to create FX texture");
		return false;
	}
	if (!CreatePickingTexture(size))
	{
		GELogger.Err("GBuffer: Failed to create Picking texture");
		return false;
	}
    return true;
}

void GBuffer::SetAsTarget(const Texture* aDepthStencil)
{
	RHI::SetRenderTargets(myTextureList, aDepthStencil);
}

void GBuffer::SetAsResource(unsigned aStage, unsigned aSlot)
{
	RHI::SetTextureResources(aStage, aSlot, myTextureList);
}

void GBuffer::UnbindAsResource(unsigned aStage, unsigned aSlot)
{
	RHI::SetTextureResources(aStage, aSlot, myNullPtrList);
}

void GBuffer::ClearTextures()
{
	RHI::ClearRenderTarget(&myAlbedoMap);
	RHI::ClearRenderTarget(&myMaterialMap);
	RHI::ClearRenderTarget(&myVertexNormalMap);
	RHI::ClearRenderTarget(&myPixelNormalMap);
	RHI::ClearRenderTarget(&myPositionMap);
	RHI::ClearRenderTarget(&myFXMap);
	RHI::ClearRenderTarget(&myPickingMap);
}

unsigned GBuffer::Count() const
{
	return static_cast<unsigned>(myTextureList.size());
}

bool GBuffer::CreateAlbedoTexture(const RHI::DeviceSize& aSize)
{
	if (!RHI::CreateTexture(&myAlbedoMap, L"GBuffer_Albedo", aSize.Width, aSize.Height, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
	{
		return false;
	}
	RHI::ClearRenderTarget(&myAlbedoMap);
	myTextureList.emplace_back(&myAlbedoMap);
	myNullPtrList.emplace_back(nullptr);
    return true;
}

bool GBuffer::CreateMaterialTexture(const RHI::DeviceSize& aSize)
{
	if (!RHI::CreateTexture(&myMaterialMap, L"GBuffer_Material", aSize.Width, aSize.Height, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
	{
		return false;
	}
	RHI::ClearRenderTarget(&myMaterialMap);
	myTextureList.emplace_back(&myMaterialMap);
	myNullPtrList.emplace_back(nullptr);
	return true;
}

bool GBuffer::CreateVertexNormalTexture(const RHI::DeviceSize& aSize)
{
	if (!RHI::CreateTexture(&myVertexNormalMap, L"GBuffer_VertexNormal", aSize.Width, aSize.Height, DXGI_FORMAT_R16G16B16A16_SNORM, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
	{
		return false;
	}
	RHI::ClearRenderTarget(&myVertexNormalMap);
	myTextureList.emplace_back(&myVertexNormalMap);
	myNullPtrList.emplace_back(nullptr);
	return true;
}

bool GBuffer::CreatePixelNormalTexture(const RHI::DeviceSize& aSize)
{
	if (!RHI::CreateTexture(&myPixelNormalMap, L"GBuffer_PixelNormal", aSize.Width, aSize.Height, DXGI_FORMAT_R16G16B16A16_SNORM, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
	{
		return false;
	}
	RHI::ClearRenderTarget(&myPixelNormalMap);
	myTextureList.emplace_back(&myPixelNormalMap);
	myNullPtrList.emplace_back(nullptr);
	return true;
}

bool GBuffer::CreatePositionTexture(const RHI::DeviceSize& aSize)
{
	if (!RHI::CreateTexture(&myPositionMap, L"GBuffer_Position", aSize.Width, aSize.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
	{
		return false;
	}
	RHI::ClearRenderTarget(&myPositionMap);
	myTextureList.emplace_back(&myPositionMap);
	myNullPtrList.emplace_back(nullptr);
	return true;
}

bool GBuffer::CreateFXTexture(const RHI::DeviceSize& aSize)
{
	if (!RHI::CreateTexture(&myFXMap, L"GBuffer_FX", aSize.Width, aSize.Height, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
	{
		return false;
	}
	RHI::ClearRenderTarget(&myFXMap);
	myTextureList.emplace_back(&myFXMap);
	myNullPtrList.emplace_back(nullptr);
	return true;
}

bool GBuffer::CreatePickingTexture(const RHI::DeviceSize& aSize)
{
	if (!RHI::CreateTexture(&myPickingMap, L"GBuffer_Picking", aSize.Width, aSize.Height, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
	{
		return false;
	}
	RHI::ClearRenderTarget(&myPickingMap);
	myTextureList.emplace_back(&myPickingMap);
	myNullPtrList.emplace_back(nullptr);
	return true;
}
