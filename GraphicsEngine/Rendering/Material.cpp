#include "GraphicsEngine.pch.h"
#include "Material.h"
#include <External/jsonCpp/json.h>
#include "AssetManager/AssetManager.h"
#include "../InterOp/Helpers.h"

Material::Material() : myVertexShader(nullptr), myPixelShader(nullptr), myTextures(), myBuffer(), myName(), myAlbedoTexture(nullptr), myNormalTexture(nullptr)
{
}

Material::Material(const Material& aMaterial) : myVertexShader(aMaterial.myVertexShader), myPixelShader(aMaterial.myPixelShader), myTextures(aMaterial.myTextures), myBuffer(aMaterial.myBuffer), myName(aMaterial.myName), myAlbedoTexture(aMaterial.myAlbedoTexture), myNormalTexture(aMaterial.myNormalTexture)
{
	myBuffer.Initialize();
}

Material::Material(const Json::Value& aJsonValue): myVertexShader(AssetManager::GetAsset<Shader*>(aJsonValue["VertexShader"].asString())), myPixelShader(AssetManager::GetAsset<Shader*>(aJsonValue["PixelShader"].asString())), 
myTextures(), myBuffer(), myName(aJsonValue["Name"].asString()), myAlbedoTexture(AssetManager::GetAsset<Texture*>(aJsonValue["AlbedoTexture"].asString())), myNormalTexture(AssetManager::GetAsset<Texture*>(aJsonValue["NormalTexture"].asString()))
{
	myBuffer.Initialize();
}

Material::Material(const std::string& aName, Shader* aVertexShader, Shader* aPixelShader, Texture* anAlbedo, Texture* aNormal) : myVertexShader(aVertexShader), myPixelShader(aPixelShader), myTextures(), myBuffer(), myName(aName), myAlbedoTexture(anAlbedo), myNormalTexture(aNormal)
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
	myBuffer = aMaterial.myBuffer; // Potential future problem with comptr copy
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

void Material::SetTexture(Texture* aTexture)
{
	myAlbedoTexture = aTexture;
}

void Material::SetNormalTexture(Texture* aTexture)
{
	myNormalTexture = aTexture;
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

const Texture* Material::GetTexture() const
{
	return myAlbedoTexture;
}

const Texture* Material::GetNormalTexture() const
{
	return myNormalTexture;
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
