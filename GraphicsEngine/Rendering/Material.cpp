#include "GraphicsEngine.pch.h"
#include "Material.h"
#include <External/jsonCpp/json.h>
#include "AssetManager/AssetManager.h"
#include "../InterOp/Helpers.h"

Material::Material() : myVertexShader(nullptr), myPixelShader(nullptr), myTextures(), myBuffer(), myName(), myAlbedoTexture(nullptr), myNormalTexture(nullptr), myMaterialTexture(nullptr), myFXTexture(nullptr), myShininess(62.f), myMetalness(), myNormalStrength(1.f), myUVTiling(1.f, 1.f), myAlbedoColor()
{
}

Material::Material(const Material& aMaterial) : myVertexShader(aMaterial.myVertexShader), myPixelShader(aMaterial.myPixelShader), myTextures(aMaterial.myTextures), myBuffer(aMaterial.myBuffer), myName(aMaterial.myName), myAlbedoTexture(aMaterial.myAlbedoTexture), myFXTexture(aMaterial.myFXTexture),
myNormalTexture(aMaterial.myNormalTexture), myMaterialTexture(aMaterial.myMaterialTexture), myShininess(aMaterial.myShininess), myMetalness(aMaterial.myMetalness), myNormalStrength(aMaterial.myNormalStrength), myUVTiling(aMaterial.myUVTiling), myAlbedoColor(aMaterial.myAlbedoColor)
{
	myBuffer.Initialize();
}

Material::Material(const Json::Value& aJsonValue) : myVertexShader(aJsonValue["VertexShader"].isNull() ? nullptr : AssetManager::GetAsset<Shader*>(aJsonValue["VertexShader"].asString())), 
myPixelShader(aJsonValue["PixelShader"].isNull() ? nullptr : AssetManager::GetAsset<Shader*>(aJsonValue["PixelShader"].asString())), myTextures(), myBuffer(), myName(aJsonValue["Name"].asString()),
myAlbedoTexture(aJsonValue["AlbedoTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["AlbedoTexture"].asString())), 
myNormalTexture(aJsonValue["NormalTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["NormalTexture"].asString())),
myMaterialTexture(aJsonValue["MaterialTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["MaterialTexture"].asString())),
myFXTexture(aJsonValue["FXTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["FXTexture"].asString())), myShininess(aJsonValue["Shininess"].asFloat()), myMetalness(aJsonValue["Metalness"].asFloat()),
myNormalStrength(aJsonValue["NormalStrength"].asFloat()), myUVTiling(aJsonValue["UVTiling"]["X"].asFloat(), aJsonValue["UVTiling"]["Y"].asFloat()),
myAlbedoColor(aJsonValue["AlbedoColor"]["R"].asFloat(), aJsonValue["AlbedoColor"]["G"].asFloat(), aJsonValue["AlbedoColor"]["B"].asFloat(), aJsonValue["AlbedoColor"]["A"].asFloat())
{
	for (unsigned i = 0; i < aJsonValue["Textures"].size(); i++)
	{
		const Json::Value& texture = aJsonValue["Textures"][i];
		myTextures.emplace_back(texture["Texture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(texture["Texture"].asString()), texture["Stage"].asUInt(), texture["Slot"].asUInt());
	}
	myBuffer.Initialize();
}

Material::Material(const std::string& aName, Shader* aVertexShader, Shader* aPixelShader, Texture* anAlbedo, Texture* aNormal, Texture* aMaterial, Texture* aFX) : myVertexShader(aVertexShader), myPixelShader(aPixelShader), myTextures(), 
myBuffer(), myName(aName), myAlbedoTexture(anAlbedo), myNormalTexture(aNormal), myMaterialTexture(aMaterial), myFXTexture(aFX), myShininess(62.f), myMetalness(), myNormalStrength(1.f), myUVTiling(1.f, 1.f), myAlbedoColor()
{
	myBuffer.Initialize();
}

Material& Material::operator=(const Material& aMaterial)
{
	myVertexShader = aMaterial.myVertexShader;
	myPixelShader = aMaterial.myPixelShader;
	myTextures = aMaterial.myTextures;
	myName = aMaterial.myName;
	myAlbedoTexture = aMaterial.myAlbedoTexture;
	myNormalTexture = aMaterial.myNormalTexture;
	myMaterialTexture = aMaterial.myMaterialTexture;
	myFXTexture = aMaterial.myFXTexture;
	myBuffer = aMaterial.myBuffer; // Potential future problem with comptr copy
	myShininess = aMaterial.myShininess;
	myMetalness = aMaterial.myMetalness;
	myNormalStrength = aMaterial.myNormalStrength;
	myUVTiling = aMaterial.myUVTiling;
	myAlbedoColor = aMaterial.myAlbedoColor;
	myBuffer.Initialize();
	return *this;
}

void Material::SetVertexShader(Shader* aShader)
{
	myVertexShader = aShader;
}

void Material::SetPixelShader(Shader* aShader)
{
	myPixelShader = aShader;
}

void Material::SetAlbedoTexture(Texture* aTexture)
{
	myAlbedoTexture = aTexture;
}

void Material::SetNormalTexture(Texture* aTexture)
{
	myNormalTexture = aTexture;
}

void Material::SetMaterialTexture(Texture* aTexture)
{
	myMaterialTexture = aTexture;
}

void Material::SetFXTexture(Texture* aTexture)
{
	myFXTexture = aTexture;
}

void Material::SetShininess(float aShininess)
{
	myShininess = aShininess;
}

void Material::SetMetalness(float aMetalness)
{
	myMetalness = aMetalness;
}

void Material::SetNormalStrength(float aNormalStrength)
{
	myNormalStrength = aNormalStrength;
}

void Material::SetUVTiling(const CommonUtilities::Vector2f& aUVTiling)
{
	myUVTiling = aUVTiling;
}

void Material::SetColor(const CommonUtilities::Vector4f& aColor)
{
	myAlbedoColor = aColor;
}

void Material::SetName(const std::string& aName)
{
	myName = aName;
}

const Shader* Material::GetVertexShader() const
{
	return myVertexShader;
}

const Shader* Material::GetPixelShader() const
{
	return myPixelShader;
}

const Texture* Material::GetAlbedoTexture() const
{
	return myAlbedoTexture;
}

const Texture* Material::GetNormalTexture() const
{
	return myNormalTexture;
}

const Texture* Material::GetMaterialTexture() const
{
	return myMaterialTexture;
}

const Texture* Material::GetFXTexture() const
{
	return myFXTexture;
}

float Material::GetShininess() const
{
	return myShininess;
}

float Material::GetMetalness() const
{
	return myMetalness;
}

float Material::GetNormalStrength() const
{
	return myNormalStrength;
}

const CommonUtilities::Vector2f& Material::GetUVTiling() const
{
	return myUVTiling;
}

const CommonUtilities::Vector4f& Material::GetColor() const
{
	return myAlbedoColor;
}

const std::string& Material::GetName() const
{
	return myName;
}

const MaterialBuffer& Material::GetBuffer() const
{
	return myBuffer;
}

Json::Value Material::ToJson() const
{
	Json::Value result;
	result["Name"] = myName;

	result["Textures"] = Json::arrayValue;
	for (unsigned i = 0; i < myTextures.size(); i++)
	{
		const TextureBinding& binding = myTextures[i];
		Json::Value texture;
		if (binding.texture)
		{
			texture["Texture"] = Helpers::string_cast<std::string>(binding.texture->GetName());
		}
		else
		{
			texture["Texture"] = Json::nullValue;
		}		
		texture["Slot"] = binding.slot;
		texture["Stage"] = binding.stage;
		result["Textures"][i] = texture;
	}

	if (myPixelShader)
	{
		result["VertexShader"] = Helpers::string_cast<std::string>(myVertexShader->GetName());
	}
	else
	{
		result["VertexShader"] = Json::nullValue;
	}	
	if (myPixelShader)
	{
		result["PixelShader"] = Helpers::string_cast<std::string>(myPixelShader->GetName());
	}
	else
	{
		result["PixelShader"] = Json::nullValue;
	}

	if (myAlbedoTexture)
	{
		result["AlbedoTexture"] = Helpers::string_cast<std::string>(myAlbedoTexture->GetName());
	}
	else
	{
		result["AlbedoTexture"] = Json::nullValue;
	}
	if (myNormalTexture)
	{
		result["NormalTexture"] = Helpers::string_cast<std::string>(myNormalTexture->GetName());
	}
	else
	{
		result["NormalTexture"] = Json::nullValue;
	}
	if (myMaterialTexture)
	{
		result["MaterialTexture"] = Helpers::string_cast<std::string>(myMaterialTexture->GetName());
	}
	else
	{
		result["MaterialTexture"] = Json::nullValue;
	}	
	if (myFXTexture)
	{
		result["FXTexture"] = Helpers::string_cast<std::string>(myFXTexture->GetName());
	}
	else
	{
		result["FXTexture"] = Json::nullValue;
	}
	
	//result["Buffer"] = myBuffer.Data;  // Add all values to separate sub names
	result["Shininess"] = myShininess;
	result["Metalness"] = myMetalness;
	result["NormalStrength"] = myNormalStrength;
	result["UVTiling"]["X"] = myUVTiling.x;
	result["UVTiling"]["Y"] = myUVTiling.y;
	result["AlbedoColor"]["R"] = myAlbedoColor.x;
	result["AlbedoColor"]["G"] = myAlbedoColor.y;
	result["AlbedoColor"]["B"] = myAlbedoColor.z;
	result["AlbedoColor"]["A"] = myAlbedoColor.w;
	return result;
}

void Material::AddTexture(Texture* aTexture, unsigned aPipelineStage, unsigned aSlot)
{
	myTextures.emplace_back(aTexture, aPipelineStage, aSlot);
}
