#include "GraphicsEngine.pch.h"
#include "Material.h"
#include <External/jsonCpp/json.h>
#include "AssetManager/AssetManager.h"
#include "../InterOp/Helpers.h"

Material::Material() : myVertexShader(nullptr), myPixelShader(nullptr), myTextures(), myBuffer(), myName(), myAlbedoTexture(nullptr), myNormalTexture(nullptr), myMaterialTexture(nullptr), myShininess(62.f), myMetalness(), myNormalStrength(1.f), myUVTiling(1.f, 1.f), myAlbedoColor()
{
}

Material::Material(const Material& aMaterial) : myVertexShader(aMaterial.myVertexShader), myPixelShader(aMaterial.myPixelShader), myTextures(aMaterial.myTextures), myBuffer(aMaterial.myBuffer), myName(aMaterial.myName), myAlbedoTexture(aMaterial.myAlbedoTexture),
myNormalTexture(aMaterial.myNormalTexture), myMaterialTexture(aMaterial.myMaterialTexture), myShininess(aMaterial.myShininess), myMetalness(aMaterial.myMetalness), myNormalStrength(aMaterial.myNormalStrength), myUVTiling(aMaterial.myUVTiling), myAlbedoColor(aMaterial.myAlbedoColor)
{
	myBuffer.Initialize();
}

Material::Material(const Json::Value& aJsonValue) : myVertexShader(AssetManager::GetAsset<Shader*>(aJsonValue["VertexShader"].asString())), myPixelShader(AssetManager::GetAsset<Shader*>(aJsonValue["PixelShader"].asString())),
myTextures(), myBuffer(), myName(aJsonValue["Name"].asString()), myAlbedoTexture(AssetManager::GetAsset<Texture*>(aJsonValue["AlbedoTexture"].asString())), myNormalTexture(AssetManager::GetAsset<Texture*>(aJsonValue["NormalTexture"].asString())),
myMaterialTexture(AssetManager::GetAsset<Texture*>(aJsonValue["MaterialTexture"].asString())), myShininess(aJsonValue["Shininess"].asFloat()), myMetalness(aJsonValue["Metalness"].asFloat()), myNormalStrength(aJsonValue["NormalStrength"].asFloat()),
myUVTiling(aJsonValue["UVTiling"]["X"].asFloat(), aJsonValue["UVTiling"]["Y"].asFloat()),
myAlbedoColor(aJsonValue["AlbedoColor"]["R"].asFloat(), aJsonValue["AlbedoColor"]["G"].asFloat(), aJsonValue["AlbedoColor"]["B"].asFloat(), aJsonValue["AlbedoColor"]["A"].asFloat())
{
	myBuffer.Initialize();
}

Material::Material(const std::string& aName, Shader* aVertexShader, Shader* aPixelShader, Texture* anAlbedo, Texture* aNormal, Texture* aMaterial) : myVertexShader(aVertexShader), myPixelShader(aPixelShader), myTextures(), myBuffer(), myName(aName), myAlbedoTexture(anAlbedo), myNormalTexture(aNormal), myMaterialTexture(aMaterial)
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
	result["VertexShader"] = Helpers::string_cast<std::string>(myVertexShader->GetName());
	result["PixelShader"] = Helpers::string_cast<std::string>(myPixelShader->GetName());
	//result["Textures"] = Json::arrayValue;  // Add all values in a loop
	//result["Buffer"]["Data"] = myBuffer.Data;  // Add all values to separate sub names
	result["AlbedoTexture"] = Helpers::string_cast<std::string>(myAlbedoTexture->GetName());
	result["NormalTexture"] = Helpers::string_cast<std::string>(myNormalTexture->GetName());
	result["MaterialTexture"] = Helpers::string_cast<std::string>(myMaterialTexture->GetName());
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

void Material::AddTexture(Texture* aTexture, unsigned aPipelineStage, eTextureSlot aSlot)
{
	myTextures.emplace_back();
	auto& binding = myTextures.back();
	binding.slot = aSlot;
	binding.stage = aPipelineStage;
	binding.texture = aTexture;
}
