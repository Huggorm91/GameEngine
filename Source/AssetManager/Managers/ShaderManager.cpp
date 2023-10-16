#include "AssetManager.pch.h"
#include "ShaderManager.h"
#include "GraphicsEngine/InterOp/RHI.h"

using namespace Crimson;

void ShaderManager::Init()
{
	myFilePaths = Crimson::GetAllFilepathsInDirectory(GetPath(), GetExtension());
}

Shader* ShaderManager::GetShader(const std::string& aPath, bool aShouldLogErrors)
{
	if (auto iter = myShaders.find(aPath); iter != myShaders.end())
	{
		return &iter->second;
	}
	else
	{
		return LoadShader(aPath, aShouldLogErrors);
	}
}

Shader* ShaderManager::LoadShader(const std::string& aPath, bool aShouldLogErrors)
{
	std::string path = Crimson::AddExtensionIfMissing(aPath, GetExtension());
	path = Crimson::GetValidPath(path, GetPath());

	if (path.empty())
	{
		if (aShouldLogErrors)
		{
			AMLogger.Warn("ShaderManager: Could not load shader from path: " + aPath);
		}
		return nullptr;
	}

	Shader shader;
	if (RHI::LoadShader(&shader, Crimson::ToWString(path)))
	{
		auto iter = myShaders.emplace(aPath, shader);
		return &iter.first->second;
	}

	if (aShouldLogErrors)
	{
		AMLogger.Err("ShaderManager: Could not load a shader from: " + aPath);
	}
	return nullptr;
}
