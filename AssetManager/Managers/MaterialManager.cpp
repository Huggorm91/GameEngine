#include "AssetManager.pch.h"
#include "MaterialManager.h"
#include "Json/jsonCpp/json.h"
#include <fstream>
#include "File/DirectoryFunctions.h"

void MaterialManager::Init()
{
	myFilePaths = Crimson::GetAllFilepathsInDirectory(GetPath(), GetExtension());
}

Material* MaterialManager::GetMaterial(const std::string& aPath, bool aShouldLogErrors)
{
	if (auto iter = myMaterials.find(aPath); iter != myMaterials.end())
	{
		return &iter->second;
	}
	else
	{
		return LoadMaterial(aPath, aShouldLogErrors);
	}
}

Material* MaterialManager::CreateMaterial(const Material& anAsset, const std::string& anIdentifier)
{
	if (auto iter = myMaterials.find(anIdentifier); iter != myMaterials.end())
	{
		iter->second = anAsset;
		return &iter->second;
	}
	else
	{
		auto newIter = myMaterials.emplace(anIdentifier, anAsset);
		return &newIter.first->second;
	}
}

Material* MaterialManager::CreateMaterial(const std::string& anIdentifier, Shader* aVertexShader, Shader* aPixelShader, Texture* anAlbedo, Texture* aNormal)
{
	if (auto iter = myMaterials.find(anIdentifier); iter != myMaterials.end())
	{
		iter->second = Material{ anIdentifier, aVertexShader, aPixelShader, anAlbedo, aNormal };
		return &iter->second;
	}
	else
	{
		auto newIter = myMaterials.emplace(anIdentifier, Material{ anIdentifier, aVertexShader, aPixelShader, anAlbedo, aNormal });
		return &newIter.first->second;
	}
}

void MaterialManager::SaveMaterial(const Material* aMaterial, const std::string& aPath)
{
	std::string path = aPath;
	if (!Crimson::HasValidExtension(aPath, GetExtension()))
	{
		path += GetExtension();
	}
	path = Crimson::CreateValidPath(path, GetPath());

	if (path.empty())
	{
		AMLogger.Warn("MaterialManager: Could not save material to path: " + aPath);
		return;
	}

	std::fstream fileStream(path, std::ios::out);
	if (fileStream)
	{
		Json::Value material = aMaterial->ToJson();
		material["Name"] = path;
		Json::StreamWriterBuilder builder;
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		writer->write(material, &fileStream);
		fileStream.flush();
	}
	else
	{
		AMLogger.Err("MaterialManager: Could not open file at: " + aPath);
	}
	fileStream.close();
}

Material* MaterialManager::LoadMaterial(const std::string& aPath, bool aShouldLogErrors)
{
	std::string path = Crimson::AddExtensionIfMissing(aPath, GetExtension());
	path = Crimson::GetValidPath(path, GetPath());
	if (path.empty())
	{
		if (aShouldLogErrors)
		{
			AMLogger.Warn("MaterialManager: Could not load material from path: " + aPath);
		}
		return nullptr;
	}

	Json::Value json;
	std::fstream fileStream(path, std::ios::in);
	if (fileStream)
	{
		fileStream >> json;
	}
	else
	{
		if (aShouldLogErrors)
		{
			AMLogger.Err("MaterialManager: Could not open file at: " + aPath);
		}
		fileStream.close();
		return nullptr;
	}
	fileStream.close();

	auto iter = myMaterials.emplace(path, json);

	return &iter.first->second;
}