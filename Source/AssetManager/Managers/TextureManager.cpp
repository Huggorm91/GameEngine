#include "AssetManager.pch.h"
#include "TextureManager.h"
#include "GraphicsEngine/InterOp/RHI.h"

using namespace Crimson;

void TextureManager::Init()
{
	UpdateFilePaths();
}

void TextureManager::LoadAllTextures()
{
	for (auto& path : myFilePaths)
	{
		LoadTexture(path, true);
	}
}

void TextureManager::UpdateFilePaths()
{
	myFilePaths = Crimson::GetAllFilepathsInDirectory(GetPath(), GetExtension(), true);
}

Texture TextureManager::GetUncachedTexture(const std::string& aPath, bool aShouldLogErrors) const
{
	std::string path = Crimson::AddExtensionIfMissing(aPath, GetExtension());
	path = Crimson::GetValidPath(path, GetPath());
	if (path.empty())
	{
		if (aShouldLogErrors)
		{
			AMLogger.Warn("TextureManager: Could not find path: " + aPath);
		}
		return Texture();
	}
	return LoadUncachedTexture(path, aShouldLogErrors);
}

Texture* TextureManager::GetTexture(const std::string& aPath, bool aShouldLogErrors)
{
	std::string path = Crimson::AddExtensionIfMissing(aPath, GetExtension());
	path = Crimson::GetValidPath(path, GetPath());
	if (path.empty())
	{
		if (aShouldLogErrors)
		{
			AMLogger.Warn("TextureManager: Could not find path: " + aPath);
		}
		return nullptr;
	}

	if (auto iter = myTextures.find(path); iter != myTextures.end())
	{
		return &iter->second;
	}
	else
	{
		return LoadTexture(path, aShouldLogErrors);
	}
}

const std::unordered_set<std::string>& TextureManager::GetTexturelist() const
{
	return myLoadedTextures;
}

Texture TextureManager::LoadUncachedTexture(const std::string& aPath, bool aShouldLogErrors) const
{
	Texture texture;
	if (RHI::LoadTexture(&texture, Crimson::ToWString(aPath)))
	{
		return texture;
	}

	if (aShouldLogErrors)
	{
		AMLogger.Err("TextureManager: Could not load a texture from: " + aPath);
	}
	return Texture();
}

Texture* TextureManager::LoadTexture(const std::string& aPath, bool aShouldLogErrors)
{
	Texture texture;
	if (RHI::LoadTexture(&texture, Crimson::ToWString(aPath)))
	{
		auto iter = myTextures.emplace(aPath, texture);
		myLoadedTextures.emplace(aPath);
		return &iter.first->second;
	}

	if (aShouldLogErrors)
	{
		AMLogger.Err("TextureManager: Could not load a texture from: " + aPath);
	}
	return nullptr;
}
