#include "AssetManager.pch.h"
#include "ShaderManager.h"
#include "GraphicsEngine/InterOp/RHI.h"

using namespace Crimson;

void ShaderManager::Init()
{
	myFilePaths = GetAllFilepathsInDirectory(GetPath(), GetExtension());
}

Shader* ShaderManager::GetShader(const std::string& aPath)
{
	if (auto iter = myShaders.find(aPath); iter != myShaders.end())
	{
		return &iter->second;
	}
	else
	{
		return LoadShader(aPath);
	}
}

Shader* ShaderManager::LoadShader(const std::string& aPath)
{
	std::string path = AddExtensionIfMissing(aPath, GetExtension());
	path = GetValidPath(path, GetPath());

	if (path.empty())
	{
		AMLogger.Err("ShaderManager: Could not load shader from path: " + aPath);
		return nullptr;
	}

	Shader shader;
	if (RHI::LoadShader(&shader, Crimson::ToWString(path)))
	{
		auto iter = myShaders.emplace(aPath, shader);
		return &iter.first->second;
	}

	AMLogger.Err("ShaderManager: Could not load a shader from: " + aPath);
	return nullptr;
}
