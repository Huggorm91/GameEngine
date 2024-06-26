#pragma once

struct ID3D11Resource;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
struct D3D11_VIEWPORT;

/**
 * \brief Represents raw texture data in some format.
 */
class Texture
{
	friend class RHI;

protected:

	// The name of this texture, for easy ID.
	std::wstring myName;

	// The actual data container for this texture.
	ComPtr<ID3D11Resource> myTexture = nullptr;

	// Different views. Which are valid and usable
	// depends on the Flags used when this texture
	// was created.
	ComPtr<ID3D11ShaderResourceView> mySRV = nullptr;

	// We can be either a Render Target or a Depth Stencil.
	ComPtr<ID3D11RenderTargetView> myRTV = nullptr;
	ComPtr<ID3D11DepthStencilView> myDSV = nullptr;

	D3D11_VIEWPORT myViewport{};

	UINT myBindFlags{};
	UINT myUsageFlags{};
	UINT myAccessFlags{};

public:
	Texture() = default;
	~Texture() = default;
	
	FORCEINLINE const std::wstring& GetName() const { return myName; }
	FORCEINLINE UINT GetBindFlags() const { return myBindFlags; }
	FORCEINLINE UINT GetUsageFlags() const { return myUsageFlags; }
	FORCEINLINE UINT GetAccessFlags() const { return myAccessFlags; }
	FORCEINLINE ComPtr<ID3D11Resource> GetResource() const { return myTexture; }
	FORCEINLINE ComPtr<ID3D11ShaderResourceView> GetSRV() const { return mySRV; }
	FORCEINLINE bool IsValid() const { return myTexture != nullptr; }

	enum TextureDimension
	{
		Unknown,
		Buffer,
		Texture1D,
		Texture2D,
		Texture3D,
	};

	FORCEINLINE TextureDimension GetType() const {
		D3D11_RESOURCE_DIMENSION dimension;
		myTexture->GetType(&dimension);
		return static_cast<TextureDimension>(dimension);
	}

	unsigned GetArraySize() const;
};