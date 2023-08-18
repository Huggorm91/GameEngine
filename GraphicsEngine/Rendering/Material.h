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
	Material(const std::string& aName, Shader* aVertexShader, Shader* aPixelShader, Texture* anAlbedo, Texture* aNormal);
	~Material() = default;
	Material& operator=(const Material& aMaterial);

	void SetVertexShader(Shader* aShader);
	void SetPixelShader(Shader* aShader);

	void SetTexture(Texture* aTexture);
	void SetNormalTexture(Texture* aTexture);

	void SetShininess(float aShininess);
	void SetMetalness(float aMetalness);
	void SetNormalStrength(float aNormalStrength);
	void SetUVTiling(const CommonUtilities::Vector2f& aUVTiling);
	void SetColor(const CommonUtilities::Vector4f& aColor);

	void SetName(const std::string& aName);

	void AddTexture(Texture* aTexture, unsigned aPipelineStage, eTextureSlot aSlot);

	const Shader* GetVertexShader() const;
	const Shader* GetPixelShader() const;

	const Texture* GetTexture() const;
	const Texture* GetNormalTexture() const;

	float GetShininess() const;
	float GetMetalness() const;
	float GetNormalStrength() const;
	const CommonUtilities::Vector2f& GetUVTiling() const;
	const CommonUtilities::Vector4f& GetColor() const;

	const std::string& GetName() const;

	const MaterialBuffer& GetBuffer() const;

	Json::Value ToJson() const;

private:
	struct TextureBinding
	{
		unsigned slot {};
		unsigned stage {};
		Texture* texture = nullptr;
	};

	Shader* myVertexShader;
	Shader* myPixelShader;

	Texture* myAlbedoTexture;
	Texture* myNormalTexture;

	float myShininess;
	float myMetalness;
	float myNormalStrength;
	CommonUtilities::Vector2f myUVTiling;
	CommonUtilities::Vector4f myAlbedoColor;

	std::string myName;
	std::vector<TextureBinding> myTextures;
	MaterialBuffer myBuffer;
};