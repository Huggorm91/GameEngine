#pragma once
#include "Texture.h"
#include "Shader.h"
#include "Buffers/MaterialBuffer.h"
#include <External/jsonCpp/json-forwards.h>

enum ePipelineStage : unsigned
{
	VertexShader = 2U,
	PixelShader = 16U
};

enum eTextureSlot : unsigned
{
	Albedo,
	Normal
};

class Material
{
public:
	Material();
	Material(const Material& aMaterial);
	Material(const Json::Value& aJsonValue);
	// Any nullptr will use GraphicEngine default
	Material(const std::string& aName, Shader* aVertexShader = nullptr, Shader* aPixelShader = nullptr, Texture* anAlbedo = nullptr, Texture* aNormal = nullptr, Texture* aMaterial = nullptr);
	~Material() = default;
	Material& operator=(const Material& aMaterial);

	void SetVertexShader(Shader* aShader);
	void SetPixelShader(Shader* aShader);

	void SetAlbedoTexture(Texture* aTexture);
	void SetNormalTexture(Texture* aTexture);
	void SetMaterialTexture(Texture* aTexture);

	// Only used for BlinnPhong
	void SetShininess(float aShininess);
	// Only used for BlinnPhong
	void SetMetalness(float aMetalness);
	void SetNormalStrength(float aNormalStrength);
	void SetUVTiling(const CommonUtilities::Vector2f& aUVTiling);
	void SetColor(const CommonUtilities::Vector4f& aColor);

	void SetName(const std::string& aName);

	void AddTexture(Texture* aTexture, unsigned aPipelineStage, unsigned aSlot);

	const Shader* GetVertexShader() const;
	const Shader* GetPixelShader() const;

	const Texture* GetAlbedoTexture() const;
	const Texture* GetNormalTexture() const;
	const Texture* GetMaterialTexture() const;

	float GetShininess() const;
	float GetMetalness() const;
	float GetNormalStrength() const;
	const CommonUtilities::Vector2f& GetUVTiling() const;
	const CommonUtilities::Vector4f& GetColor() const;

	const std::string& GetName() const;

	const MaterialBuffer& GetBuffer() const;

	Json::Value ToJson() const;

private:
	friend class GfxCmd_RenderMesh;

	struct TextureBinding
	{
		unsigned slot {};
		unsigned stage {};
		Texture* texture = nullptr;

		TextureBinding(Texture* aTexture, unsigned aStage, unsigned aSlot): texture(aTexture), stage(aStage), slot(aSlot) {}
	};

	Shader* myVertexShader;
	Shader* myPixelShader;

	Texture* myAlbedoTexture;
	Texture* myNormalTexture;
	Texture* myMaterialTexture;

	float myShininess;	// Only used for BlinnPhong
	float myMetalness;	// Only used for BlinnPhong
	float myNormalStrength;
	CommonUtilities::Vector2f myUVTiling;
	CommonUtilities::Vector4f myAlbedoColor;

	std::string myName;
	std::vector<TextureBinding> myTextures;
	MaterialBuffer myBuffer;
};