#include "AssetManager.pch.h"
#include "TextureManager.h"
#include "GraphicsEngine/InterOp/Helpers.h"
#include "GraphicsEngine/InterOp/RHI.h"
#include "../DirectoryFunctions.h"

TextureManager::TextureManager(const std::string& aPath): myPath(aPath), myFilePaths(), myTextures()
{
}

void TextureManager::Init()
{
	myFilePaths = GetAllFilepathsInDirectory(myPath);
}

Texture* TextureManager::GetTexture(const std::string& aPath)
{
	if (auto iter = myTextures.find(aPath); iter != myTextures.end())
	{
		return &iter->second;
	}
	else
	{
		return LoadTexture(aPath);
	}
}

Texture* TextureManager::LoadTexture(const std::string& aPath)
{
	Texture texture;
	if (RHI::LoadTexture(&texture, Helpers::string_cast<std::wstring>(aPath)))
	{
		auto iter = myTextures.emplace(aPath, texture);
		return &iter.first->second;
	}

	AMLogger.Err("TextureManager: Could not load a texture from: " + aPath);
	return nullptr;
}
