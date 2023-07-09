#include "AssetManager.pch.h"
#include "ShaderManager.h"
#include "../DirectoryFunctions.h"
#include "GraphicsEngine/InterOp/RHI.h"
#include "GraphicsEngine/InterOp/Helpers.h"

ShaderManager::ShaderManager(const std::string& aPath): myPath(aPath), myFilePaths(), myShaders()
{
}

void ShaderManager::Init()
{
	myFilePaths = GetAllFilepathsInDirectory(myPath);
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
	Shader shader;
	if (RHI::LoadShader(&shader, Helpers::string_cast<std::wstring>(aPath)))
	{
		auto iter = myShaders.emplace(aPath, shader);
		return &iter.first->second;
	}

	AMLogger.Err("ShaderManager: Could not load a shader from: " + aPath);
	return nullptr;
}
