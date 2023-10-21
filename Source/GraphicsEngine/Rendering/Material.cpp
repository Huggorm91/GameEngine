#include "GraphicsEngine.pch.h"
#include "Material.h"
#include "../GraphicsEngine.h"
#include "Json/JsonVector.hpp"
#include "AssetManager/AssetManager.h"
#include "File/DirectoryFunctions.h"

Material::Material() : myVertexShader(nullptr), myPixelShader(nullptr), myTextures(), myBuffer(), myName(), myAlbedoTexture(nullptr), myNormalTexture(nullptr), myMaterialTexture(nullptr), myFXTexture(nullptr)
{
}

Material::Material(const Material& aMaterial) : myVertexShader(aMaterial.myVertexShader), myPixelShader(aMaterial.myPixelShader), myTextures(aMaterial.myTextures), myBuffer(aMaterial.myBuffer), myName(aMaterial.myName), myAlbedoTexture(aMaterial.myAlbedoTexture), myFXTexture(aMaterial.myFXTexture),
myNormalTexture(aMaterial.myNormalTexture), myMaterialTexture(aMaterial.myMaterialTexture)
{
	myBuffer.Initialize(L"MaterialBuffer");
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
	myBuffer.Data.UVTiling = Crimson::Vector2f(aJsonValue["UVTiling"]);
	myBuffer.Data.AlbedoColor = Crimson::Vector4f(aJsonValue["AlbedoColor"]);
	myBuffer.Data.EmissionColor = Crimson::Vector4f(aJsonValue["EmissionColor"]);
	myBuffer.Data.EmissionIntensity = aJsonValue["EmissionIntensity"].asFloat();

	for (unsigned i = 0; i < aJsonValue["Textures"].size(); i++)
	{
		const Json::Value& texture = aJsonValue["Textures"][i];
		myTextures.emplace_back(texture["Texture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(texture["Texture"].asString()), texture["Stage"].asUInt(), texture["Slot"].asUInt());
	}
	myBuffer.Initialize(L"MaterialBuffer");
}

Material::Material(const std::string& aName, Shader* aVertexShader, Shader* aPixelShader, Texture* anAlbedo, Texture* aNormal, Texture* aMaterial, Texture* aFX) : myVertexShader(aVertexShader), myPixelShader(aPixelShader), myTextures(),
myBuffer(), myName(aName), myAlbedoTexture(anAlbedo), myNormalTexture(aNormal), myMaterialTexture(aMaterial), myFXTexture(aFX)
{
	myBuffer.Initialize(L"MaterialBuffer");
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
	myBuffer.Initialize(L"MaterialBuffer");
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

void Material::SetUVTiling(const Crimson::Vector2f& aUVTiling)
{
	myBuffer.Data.UVTiling = aUVTiling;
}

void Material::SetAlbedoColor(const Crimson::Vector4f& aColor)
{
	myBuffer.Data.AlbedoColor = aColor;
}

void Material::SetEmissionColor(const Crimson::Vector4f& aColor)
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

const Crimson::Vector2f& Material::GetUVTiling() const
{
	return myBuffer.Data.UVTiling;
}

const Crimson::Vector4f& Material::GetAlbedoColor() const
{
	return myBuffer.Data.AlbedoColor;
}

const Crimson::Vector4f& Material::GetEmissionColor() const
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
			texture["Texture"] = Crimson::ToString(binding.texture->GetName());
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
		result["VertexShader"] = Crimson::GetFileName(Crimson::ToString(myVertexShader->GetName()));

		const std::string& comment = "// Only use 'ShaderName.cso' and not full path";
		result["VertexShader"].setComment(comment, Json::commentAfterOnSameLine);
	}
	else
	{
		result["VertexShader"] = Json::nullValue;
	}
	if (myPixelShader)
	{
		result["PixelShader"] = Crimson::GetFileName(Crimson::ToString(myPixelShader->GetName()));

		const std::string& comment = "// Only use 'ShaderName.cso' and not full path";
		result["PixelShader"].setComment(comment, Json::commentAfterOnSameLine);
	}
	else
	{
		result["PixelShader"] = Json::nullValue;
	}

	if (myAlbedoTexture)
	{
		result["AlbedoTexture"] = Crimson::ToString(myAlbedoTexture->GetName());
	}
	else
	{
		result["AlbedoTexture"] = Json::nullValue;
	}
	if (myNormalTexture)
	{
		result["NormalTexture"] = Crimson::ToString(myNormalTexture->GetName());
	}
	else
	{
		result["NormalTexture"] = Json::nullValue;
	}
	if (myMaterialTexture)
	{
		result["MaterialTexture"] = Crimson::ToString(myMaterialTexture->GetName());
	}
	else
	{
		result["MaterialTexture"] = Json::nullValue;
	}
	if (myFXTexture)
	{
		result["FXTexture"] = Crimson::ToString(myFXTexture->GetName());
	}
	else
	{
		result["FXTexture"] = Json::nullValue;
	}

	result["Shininess"] = myBuffer.Data.Shininess;
	result["Metalness"] = myBuffer.Data.Metalness;
	result["NormalStrength"] = myBuffer.Data.NormalStrength;
	result["UVTiling"] = static_cast<Json::Value>(myBuffer.Data.UVTiling);
	result["AlbedoColor"] = myBuffer.Data.AlbedoColor.ToJsonColor();
	result["EmissionColor"] = myBuffer.Data.EmissionColor.ToJsonColor();
	result["EmissionIntensity"] = myBuffer.Data.EmissionIntensity;
	return result;
}

void Material::Serialize(std::ostream& aStream) const
{
	aStream.write(myName.c_str(), myName.size() + 1);

	size_t pathSize = myVertexShader ? Crimson::GetFileName(Crimson::ToString(myVertexShader->GetName())).size() + 1 : 1;
	aStream.write(myVertexShader ? Crimson::GetFileName(Crimson::ToString(myVertexShader->GetName())).c_str() : "\0", pathSize);

	pathSize = myPixelShader ? Crimson::GetFileName(Crimson::ToString(myPixelShader->GetName())).size() + 1 : 1;
	aStream.write(myPixelShader ? Crimson::GetFileName(Crimson::ToString(myPixelShader->GetName())).c_str() : "\0", pathSize);

	pathSize = myAlbedoTexture ? Crimson::ToString(myAlbedoTexture->GetName()).size() + 1 : 1;
	aStream.write(myAlbedoTexture ? Crimson::ToString(myAlbedoTexture->GetName()).c_str() : "\0", pathSize);

	pathSize = myNormalTexture ? Crimson::ToString(myNormalTexture->GetName()).size() + 1 : 1;
	aStream.write(myNormalTexture ? Crimson::ToString(myNormalTexture->GetName()).c_str() : "\0", pathSize);

	pathSize = myMaterialTexture ? Crimson::ToString(myMaterialTexture->GetName()).size() + 1 : 1;
	aStream.write(myMaterialTexture ? Crimson::ToString(myMaterialTexture->GetName()).c_str() : "\0", pathSize);

	pathSize = myFXTexture ? Crimson::ToString(myFXTexture->GetName()).size() + 1 : 1;
	aStream.write(myFXTexture ? Crimson::ToString(myFXTexture->GetName()).c_str() : "\0", pathSize);

	aStream.write(reinterpret_cast<const char*>(&myBuffer.Data), sizeof(myBuffer.Data));

	size_t textureCount = myTextures.size();
	aStream.write(reinterpret_cast<const char*>(&textureCount), sizeof(textureCount));

	for (auto& binding : myTextures)
	{
		pathSize = binding.texture ? Crimson::ToString(binding.texture->GetName()).size() + 1 : 1;
		aStream.write(binding.texture ? Crimson::ToString(binding.texture->GetName()).c_str() : "\0", pathSize);
		aStream.write(reinterpret_cast<const char*>(&binding.slot), sizeof(binding.slot));
		aStream.write(reinterpret_cast<const char*>(&binding.stage), sizeof(binding.stage));
	}
}

void Material::Deserialize(std::istream& aStream)
{
	std::getline(aStream, myName, '\0');

	std::string path;
	std::getline(aStream, path, '\0');
	myVertexShader = path.empty() ? nullptr : AssetManager::GetAsset<Shader*>(path);

	std::getline(aStream, path, '\0');
	myPixelShader = path.empty() ? nullptr : AssetManager::GetAsset<Shader*>(path);

	std::getline(aStream, path, '\0');
	myAlbedoTexture = path.empty() ? nullptr : AssetManager::GetAsset<Texture*>(path);

	std::getline(aStream, path, '\0');
	myNormalTexture = path.empty() ? nullptr : AssetManager::GetAsset<Texture*>(path);

	std::getline(aStream, path, '\0');
	myMaterialTexture = path.empty() ? nullptr : AssetManager::GetAsset<Texture*>(path);

	std::getline(aStream, path, '\0');
	myFXTexture = path.empty() ? nullptr : AssetManager::GetAsset<Texture*>(path);

	aStream.read(reinterpret_cast<char*>(&myBuffer.Data), sizeof(myBuffer.Data));

	size_t textureCount = 0;
	aStream.read(reinterpret_cast<char*>(&textureCount), sizeof(textureCount));

	for (size_t i = 0; i < textureCount; i++)
	{
		auto& binding = myTextures.emplace_back();
		std::getline(aStream, path, '\0');
		binding.texture = path.empty() ? nullptr : AssetManager::GetAsset<Texture*>(path);
		aStream.read(reinterpret_cast<char*>(&binding.slot), sizeof(binding.slot));
		aStream.read(reinterpret_cast<char*>(&binding.stage), sizeof(binding.stage));
	}
	myBuffer.Initialize(L"MaterialBuffer");
}

void Material::AddTexture(Texture* aTexture, unsigned aPipelineStage, unsigned aSlot)
{
	myTextures.emplace_back(aTexture, aPipelineStage, aSlot);
}
