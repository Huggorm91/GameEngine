#include "GraphicsEngine.pch.h"
#include "Material.h"
#include <External/jsonCpp/json.h>
#include "AssetManager/AssetManager.h"
#include "../InterOp/Helpers.h"

Material::Material() : myVertexShader(nullptr), myPixelShader(nullptr), myTextures(), myBuffer(), myName(), myAlbedoTexture(nullptr), myNormalTexture(nullptr), myMaterialTexture(nullptr), myFXTexture(nullptr)
{
}

Material::Material(const Material& aMaterial) : myVertexShader(aMaterial.myVertexShader), myPixelShader(aMaterial.myPixelShader), myTextures(aMaterial.myTextures), myBuffer(aMaterial.myBuffer), myName(aMaterial.myName), myAlbedoTexture(aMaterial.myAlbedoTexture), myFXTexture(aMaterial.myFXTexture),
myNormalTexture(aMaterial.myNormalTexture), myMaterialTexture(aMaterial.myMaterialTexture)
{
	myBuffer.Initialize();
}

Material::Material(const Json::Value& aJsonValue) : myVertexShader(aJsonValue["VertexShader"].isNull() ? nullptr : AssetManager::GetAsset<Shader*>(aJsonValue["VertexShader"].asString())), 
myPixelShader(aJsonValue["PixelShader"].isNull() ? nullptr : AssetManager::GetAsset<Shader*>(aJsonValue["PixelShader"].asString())), myTextures(), myBuffer(), myName(aJsonValue["Name"].asString()),
myAlbedoTexture(aJsonValue["AlbedoTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["AlbedoTexture"].asString())), 
myNormalTexture(aJsonValue["NormalTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["NormalTexture"].asString())),
myMaterialTexture(aJsonValue["MaterialTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["MaterialTexture"].asString())),
myFXTexture(aJsonValue["FXTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["FXTexture"].asString()))
{
	myBuffer.Data.Shininess = aJsonValue["Shininess"].asFloat();
	myBuffer.Data.Metalness = aJsonValue["Metalness"].asFloat();
	myBuffer.Data.NormalStrength = aJsonValue["NormalStrength"].asFloat();
	myBuffer.Data.UVTiling = CommonUtilities::Vector2f(aJsonValue["UVTiling"]);
	myBuffer.Data.AlbedoColor = CommonUtilities::Vector4f(aJsonValue["AlbedoColor"]["R"].asFloat(), aJsonValue["AlbedoColor"]["G"].asFloat(), aJsonValue["AlbedoColor"]["B"].asFloat(), aJsonValue["AlbedoColor"]["A"].asFloat());
	myBuffer.Data.EmissionColor = CommonUtilities::Vector3f(aJsonValue["EmissionColor"]["R"].asFloat(), aJsonValue["EmissionColor"]["G"].asFloat(), aJsonValue["EmissionColor"]["B"].asFloat());
	myBuffer.Data.EmissionIntensity = aJsonValue["EmissionIntensity"].asFloat();

	for (unsigned i = 0; i < aJsonValue["Textures"].size(); i++)
	{
		const Json::Value& texture = aJsonValue["Textures"][i];
		myTextures.emplace_back(texture["Texture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(texture["Texture"].asString()), texture["Stage"].asUInt(), texture["Slot"].asUInt());
	}
	myBuffer.Initialize();
}

Material::Material(const std::string& aName, Shader* aVertexShader, Shader* aPixelShader, Texture* anAlbedo, Texture* aNormal, Texture* aMaterial, Texture* aFX) : myVertexShader(aVertexShader), myPixelShader(aPixelShader), myTextures(), 
myBuffer(), myName(aName), myAlbedoTexture(anAlbedo), myNormalTexture(aNormal), myMaterialTexture(aMaterial), myFXTexture(aFX)
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
	myBuffer = aMaterial.myBuffer;
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
	myBuffer.Data.Shininess = aShininess;
}

void Material::SetMetalness(float aMetalness)
{
	myBuffer.Data.Metalness = aMetalness;
}

void Material::SetNormalStrength(float aNormalStrength)
{
	myBuffer.Data.NormalStrength = aNormalStrength;
}

void Material::SetUVTiling(const CommonUtilities::Vector2f& aUVTiling)
{
	myBuffer.Data.UVTiling = aUVTiling;
}

void Material::SetAlbedoColor(const CommonUtilities::Vector4f& aColor)
{
	myBuffer.Data.AlbedoColor = aColor;
}

void Material::SetEmissionColor(const CommonUtilities::Vector3f& aColor)
{
	myBuffer.Data.EmissionColor = aColor;
}

void Material::SetEmissionIntensity(float anIntensity)
{
	myBuffer.Data.EmissionIntensity = anIntensity;
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
	return myBuffer.Data.Shininess;
}

float Material::GetMetalness() const
{
	return myBuffer.Data.Metalness;
}

float Material::GetNormalStrength() const
{
	return myBuffer.Data.NormalStrength;
}

const CommonUtilities::Vector2f& Material::GetUVTiling() const
{
	return myBuffer.Data.UVTiling;
}

const CommonUtilities::Vector4f& Material::GetAlbedoColor() const
{
	return myBuffer.Data.AlbedoColor;
}

const CommonUtilities::Vector3f& Material::GetEmissionColor() const
{
	return myBuffer.Data.EmissionColor;
}

float Material::GetEmissionIntensity() const
{
	return myBuffer.Data.EmissionIntensity;
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

	if (myVertexShader)
	{
		std::string path = Helpers::string_cast<std::string>(myVertexShader->GetName());
		size_t lastSlash = path.find_last_of('/') + 1;
		result["VertexShader"] = path.substr(lastSlash);

		const std::string& comment = "// Only use 'ShaderName.cso' and not full path";
		result["VertexShader"].setComment(comment, Json::commentAfterOnSameLine);
	}
	else
	{
		result["VertexShader"] = Json::nullValue;
	}	
	if (myPixelShader)
	{
		std::string path = Helpers::string_cast<std::string>(myPixelShader->GetName());
		size_t lastSlash = path.find_last_of('/') + 1;
		result["PixelShader"] = path.substr(lastSlash);

		const std::string& comment = "// Only use 'ShaderName.cso' and not full path";
		result["PixelShader"].setComment(comment, Json::commentAfterOnSameLine);
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
	
	result["Shininess"] = myBuffer.Data.Shininess;
	result["Metalness"] = myBuffer.Data.Metalness;
	result["NormalStrength"] = myBuffer.Data.NormalStrength;
	result["UVTiling"]["X"] = myBuffer.Data.UVTiling.x;
	result["UVTiling"]["Y"] = myBuffer.Data.UVTiling.y;
	result["AlbedoColor"]["R"] = myBuffer.Data.AlbedoColor.x;
	result["AlbedoColor"]["G"] = myBuffer.Data.AlbedoColor.y;
	result["AlbedoColor"]["B"] = myBuffer.Data.AlbedoColor.z;
	result["AlbedoColor"]["A"] = myBuffer.Data.AlbedoColor.w;
	result["EmissionColor"]["R"] = myBuffer.Data.EmissionColor.x;
	result["EmissionColor"]["G"] = myBuffer.Data.EmissionColor.y;
	result["EmissionColor"]["B"] = myBuffer.Data.EmissionColor.z;
	result["EmissionIntensity"] = myBuffer.Data.EmissionIntensity;
	return result;
}

void Material::AddTexture(Texture* aTexture, unsigned aPipelineStage, unsigned aSlot)
{
	myTextures.emplace_back(aTexture, aPipelineStage, aSlot);
}
