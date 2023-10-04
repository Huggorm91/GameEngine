#include "AssetManager.pch.h"
#include "TextureManager.h"
#include "GraphicsEngine/InterOp/Helpers.h"
#include "GraphicsEngine/InterOp/RHI.h"
#include "File/DirectoryFunctions.h"

void TextureManager::Init()
{
	myFilePaths = Crimson::GetAllFilepathsInDirectory(GetPath(), GetExtension());
}

void TextureManager::LoadAllTextures()
{
	for (auto& path : myFilePaths)
	{
		LoadTexture(path);
	}
}

Texture* TextureManager::GetTexture(const std::string& aPath)
{
	std::string path = Crimson::AddExtensionIfMissing(aPath, GetExtension());
	path = Crimson::GetValidPath(path, GetPath());
	if (path.empty())
	{
		AMLogger.Err("TextureManager: Could not find path: " + aPath);
		return nullptr;
	}

	if (auto iter = myTextures.find(path); iter != myTextures.end())
	{
		return &iter->second;
	}
	else
	{
		return LoadTexture(path);
	}
}

const std::unordered_set<std::string>& TextureManager::GetTexturelist() const
{
	return myLoadedTextures;
}

Texture* TextureManager::LoadTexture(const std::string& aPath)
{
	Texture texture;
	if (RHI::LoadTexture(&texture, Helpers::string_cast<std::wstring>(aPath)))
	{
		auto iter = myTextures.emplace(aPath, texture);
		myLoadedTextures.emplace(aPath);
		return &iter.first->second;
	}

	AMLogger.Err("TextureManager: Could not load a texture from: " + aPath);
	return nullptr;
}
