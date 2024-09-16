#pragma once
#include "BoxSphereBounds.h"
#ifndef NETWORK_SERVER
#include "GraphicsEngine/Rendering/Material.h"
#else
// TODO: Fix a proper way to handle Textures while running a server
class Texture
{
protected:
public:
	std::wstring myName;

	Texture() = default;
	~Texture() = default;

	FORCEINLINE const std::wstring& GetName() const { return myName; }
};

// TODO: Fix a proper way to handle Shaders while running a server
class Shader
{
public:
	std::wstring myName;
	FORCEINLINE const std::wstring& GetName() const { return myName; }
};

// TODO: Fix a proper way to handle Materials while running a server
class Material
{
public:
	Material();
	Material(const Material& aMaterial);
	Material(const Json::Value& aJsonValue);
	Material(const std::string& aName, Shader* aVertexShader = nullptr, Shader* aPixelShader = nullptr, Texture* anAlbedo = nullptr, Texture* aNormal = nullptr, Texture* aMaterial = nullptr, Texture* aFX = nullptr);
	~Material() = default;
	Material& operator=(const Material& aMaterial);

	void SetVertexShader(Shader* aShader);
	void SetPixelShader(Shader* aShader);

	void SetAlbedoTexture(Texture* aTexture);
	void SetNormalTexture(Texture* aTexture);
	void SetMaterialTexture(Texture* aTexture);
	void SetFXTexture(Texture* aTexture);

	// Only used for BlinnPhong
	void SetShininess(float aShininess);
	// Only used for BlinnPhong
	void SetMetalness(float aMetalness);
	void SetNormalStrength(float aNormalStrength);
	void SetUVTiling(const Crimson::Vector2f& aUVTiling);
	void SetAlbedoColor(const Crimson::Vector4f& aColor);
	void SetEmissionColor(const Crimson::Vector4f& aColor);
	void SetEmissionIntensity(float anIntensity);

	void SetName(const std::string& aName);

	Json::Value ToJson() const;

	void Serialize(std::ostream& aStream) const;
	void Deserialize(std::istream& aStream);

private:
	friend class GfxCmd_RenderMesh;
	friend class MeshComponent;

	struct TextureBinding
	{
		unsigned slot{};
		unsigned stage{};
		Texture* texture = nullptr;

		TextureBinding() = default;
		TextureBinding(Texture* aTexture, unsigned aStage, unsigned aSlot) : texture(aTexture), stage(aStage), slot(aSlot) {}
	};

	Shader* myVertexShader;
	Shader* myPixelShader;

	Texture* myAlbedoTexture;
	Texture* myNormalTexture;
	Texture* myMaterialTexture;
	Texture* myFXTexture;

	std::string myName;
	std::vector<TextureBinding> myTextures;

	// Replacement for buffer
	struct MaterialBufferData
	{
		Crimson::Vector4f AlbedoColor{};
		Crimson::Vector4f EmissionColor{};
		float EmissionIntensity{};
		Crimson::Vector2f UVTiling{};
		float NormalStrength{};
		float Shininess{};
		float Metalness{};

		Crimson::Vector2f padding;
	} myBuffer;
};
#endif // !NETWORK_SERVER

struct MeshData
{
	MeshData();
#ifndef NETWORK_SERVER
	MeshData(ComPtr<ID3D11Buffer> aVertexBuffer, ComPtr<ID3D11Buffer> anIndexBuffer, UINT aVertexCount, UINT anIndexCount, UINT aPrimitiveTopology, UINT aStride);
#endif // !NETWORK_SERVER
	MeshData(const TGA::FBX::Mesh::Element& anElement);
	MeshData(const MeshData& anElement);

	BoxSphereBounds myBoxSphereBounds;
	std::string myMeshName;

#ifndef NETWORK_SERVER
	ComPtr<ID3D11Buffer> myVertexBuffer;
	ComPtr<ID3D11Buffer> myIndexBuffer;
	UINT myVertexCount;
	UINT myIndexCount;
	UINT myPrimitiveTopology;
	UINT myStride;
#endif // !NETWORK_SERVER
};

struct MeshElement
{
	MeshElement();
	MeshElement(const MeshData& someData);
	MeshElement(const MeshElement& anElement);

	Material myMaterial;
	const MeshData* myData;
};