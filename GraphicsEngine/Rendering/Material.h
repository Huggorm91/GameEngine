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

	void SetName(const std::string& aName);

	void AddTexture(Texture* aTexture, unsigned aPipelineStage, eTextureSlot aSlot);

	const Shader* GetVertexShader() const;
	const Shader* GetPixelShader() const;

	const Texture* GetTexture() const;
	const Texture* GetNormalTexture() const;

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

	MaterialBuffer myBuffer;
	std::string myName;
	std::vector<TextureBinding> myTextures;
};