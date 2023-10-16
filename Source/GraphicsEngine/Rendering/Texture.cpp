#include "GraphicsEngine.pch.h"
#include "Texture.h"

unsigned Texture::GetArraySize() const
{
	switch (GetType())
	{
	case Texture::Buffer:
	{
		return 1;
	}
	case Texture::Texture1D:
	{
		ComPtr<ID3D11Texture1D> ptr1D;
		myTexture.As(&ptr1D);
		D3D11_TEXTURE1D_DESC desc1D;
		ptr1D->GetDesc(&desc1D);
		return desc1D.ArraySize;
	}
	case Texture::Texture2D:
	{
		ComPtr<ID3D11Texture2D> ptr2D;
		myTexture.As(&ptr2D);
		D3D11_TEXTURE2D_DESC desc2D;
		ptr2D->GetDesc(&desc2D);
		return desc2D.ArraySize;
	}
	case Texture::Texture3D:
	{
		return 1;
	}
	default:
		return 0;
	}
}