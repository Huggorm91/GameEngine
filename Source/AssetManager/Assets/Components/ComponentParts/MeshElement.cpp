#include "AssetManager.pch.h"
#include "MeshElement.h"
#ifndef NETWORK_SERVER
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Rendering/Vertex.h"
#else
#include "AssetManager.h"

Material::Material() : myVertexShader(nullptr), myPixelShader(nullptr), myTextures(), myBuffer(), myName(), myAlbedoTexture(nullptr), myNormalTexture(nullptr), myMaterialTexture(nullptr), myFXTexture(nullptr)
{
}

Material::Material(const Material& aMaterial) : myVertexShader(aMaterial.myVertexShader), myPixelShader(aMaterial.myPixelShader), myTextures(aMaterial.myTextures), myBuffer(aMaterial.myBuffer), myName(aMaterial.myName), myAlbedoTexture(aMaterial.myAlbedoTexture), myFXTexture(aMaterial.myFXTexture),
myNormalTexture(aMaterial.myNormalTexture), myMaterialTexture(aMaterial.myMaterialTexture)
{
}

Material::Material(const Json::Value& aJsonValue) : myVertexShader(aJsonValue["VertexShader"].isNull() ? nullptr : AssetManager::GetAsset<Shader*>(aJsonValue["VertexShader"].asString())),
myPixelShader(aJsonValue["PixelShader"].isNull() ? nullptr : AssetManager::GetAsset<Shader*>(aJsonValue["PixelShader"].asString())), myTextures(), myBuffer(), myName(aJsonValue["Name"].asString()),
myAlbedoTexture(aJsonValue["AlbedoTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["AlbedoTexture"].asString())),
myNormalTexture(aJsonValue["NormalTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["NormalTexture"].asString())),
myMaterialTexture(aJsonValue["MaterialTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["MaterialTexture"].asString())),
myFXTexture(aJsonValue["FXTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["FXTexture"].asString()))
{
	myBuffer.Shininess = aJsonValue["Shininess"].asFloat();
	myBuffer.Metalness = aJsonValue["Metalness"].asFloat();
	myBuffer.NormalStrength = aJsonValue["NormalStrength"].asFloat();
	myBuffer.UVTiling = Crimson::Vector2f(aJsonValue["UVTiling"]);
	myBuffer.AlbedoColor = Crimson::Vector4f(aJsonValue["AlbedoColor"]);
	myBuffer.EmissionColor = Crimson::Vector4f(aJsonValue["EmissionColor"]);
	myBuffer.EmissionIntensity = aJsonValue["EmissionIntensity"].asFloat();

	for (unsigned i = 0; i < aJsonValue["Textures"].size(); i++)
	{
		const Json::Value& texture = aJsonValue["Textures"][i];
		myTextures.emplace_back(texture["Texture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(texture["Texture"].asString()), texture["Stage"].asUInt(), texture["Slot"].asUInt());
	}
}

Material::Material(const std::string& aName, Shader* aVertexShader, Shader* aPixelShader, Texture* anAlbedo, Texture* aNormal, Texture* aMaterial, Texture* aFX) : myVertexShader(aVertexShader), myPixelShader(aPixelShader), myTextures(),
myBuffer(), myName(aName), myAlbedoTexture(anAlbedo), myNormalTexture(aNormal), myMaterialTexture(aMaterial), myFXTexture(aFX)
{
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
	myBuffer.Shininess = aShininess;
}

void Material::SetMetalness(float aMetalness)
{
	myBuffer.Metalness = aMetalness;
}

void Material::SetNormalStrength(float aNormalStrength)
{
	myBuffer.NormalStrength = aNormalStrength;
}

void Material::SetUVTiling(const Crimson::Vector2f& aUVTiling)
{
	myBuffer.UVTiling = aUVTiling;
}

void Material::SetAlbedoColor(const Crimson::Vector4f& aColor)
{
	myBuffer.AlbedoColor = aColor;
}

void Material::SetEmissionColor(const Crimson::Vector4f& aColor)
{
	myBuffer.EmissionColor = aColor;
}

void Material::SetEmissionIntensity(float anIntensity)
{
	myBuffer.EmissionIntensity = anIntensity;
}

void Material::SetName(const std::string& aName)
{
	myName = aName;
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

	result["Shininess"] = myBuffer.Shininess;
	result["Metalness"] = myBuffer.Metalness;
	result["NormalStrength"] = myBuffer.NormalStrength;
	result["UVTiling"] = static_cast<Json::Value>(myBuffer.UVTiling);
	result["AlbedoColor"] = myBuffer.AlbedoColor.ToJsonColor();
	result["EmissionColor"] = myBuffer.EmissionColor.ToJsonColor();
	result["EmissionIntensity"] = myBuffer.EmissionIntensity;
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

	aStream.write(reinterpret_cast<const char*>(&myBuffer), sizeof(myBuffer));

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

	aStream.read(reinterpret_cast<char*>(&myBuffer), sizeof(myBuffer));

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
}
#endif // !NETWORK_SERVER

MeshElement::MeshElement() :
#ifndef NETWORK_SERVER
	myMaterial(GraphicsEngine::Get().GetDefaultMaterial()),
#else
	myMaterial(AssetManager::GetAsset<Material>("Default/Default_Material.mat")),
#endif // !NETWORK_SERVER
	myData(nullptr)
{
}

MeshElement::MeshElement(const MeshData& someData) :
#ifndef NETWORK_SERVER
	myMaterial(GraphicsEngine::Get().GetDefaultMaterial()),
#else
	myMaterial(AssetManager::GetAsset<Material>("Default/Default_Material.mat")),
#endif // !NETWORK_SERVER
	myData(&someData)
{
}

MeshElement::MeshElement(const MeshElement& anElement) :
	myMaterial(anElement.myMaterial),
	myData(anElement.myData)
{
}

MeshData::MeshData() :
	myBoxSphereBounds(),
	myMeshName()
#ifndef NETWORK_SERVER
	, myVertexBuffer(),
	myIndexBuffer(),
	myVertexCount(),
	myIndexCount(),
	myPrimitiveTopology(),
	myStride()
#endif // !NETWORK_SERVER
{
}

#ifndef NETWORK_SERVER
MeshData::MeshData(ComPtr<ID3D11Buffer> aVertexBuffer, ComPtr<ID3D11Buffer> anIndexBuffer, UINT aVertexCount, UINT anIndexCount, UINT aPrimitiveTopology, UINT aStride) :
	myVertexBuffer(aVertexBuffer), myIndexBuffer(anIndexBuffer), myVertexCount(aVertexCount), myIndexCount(anIndexCount), myPrimitiveTopology(aPrimitiveTopology), myStride(aStride), myBoxSphereBounds(), myMeshName()
{
}
#endif // !NETWORK_SERVER

MeshData::MeshData(const TGA::FBX::Mesh::Element& anElement) :
	myBoxSphereBounds(anElement.BoxSphereBounds),
	myMeshName(anElement.MeshName)
#ifndef NETWORK_SERVER
	, myVertexBuffer(),
	myIndexBuffer(),
	myVertexCount(static_cast<UINT>(anElement.Vertices.size())),
	myIndexCount(static_cast<UINT>(anElement.Indices.size())),
	myPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	myStride(sizeof(Vertex))
#endif // !NETWORK_SERVER
{
	if (anElement.BoxBounds.IsValid && anElement.BoxSphereBounds.Radius == 0.f)
	{
		myBoxSphereBounds = anElement.BoxBounds;
	}
}

MeshData::MeshData(const MeshData& anElement) :
	myBoxSphereBounds(anElement.myBoxSphereBounds),
	myMeshName(anElement.myMeshName)
#ifndef NETWORK_SERVER
	, myVertexBuffer(anElement.myVertexBuffer),
	myIndexBuffer(anElement.myIndexBuffer),
	myVertexCount(anElement.myVertexCount),
	myIndexCount(anElement.myIndexCount),
	myPrimitiveTopology(anElement.myPrimitiveTopology),
	myStride(anElement.myStride)
#endif // !NETWORK_SERVER
{
}
