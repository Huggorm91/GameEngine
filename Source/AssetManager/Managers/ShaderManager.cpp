#include "AssetManager.pch.h"
#include "ShaderManager.h"
#ifndef NETWORK_SERVER
#include "GraphicsEngine/InterOp/RHI.h"
#endif // !NETWORK_SERVER

using namespace Crimson;

void ShaderManager::Init()
{
	UpdateFilePaths();
}

void ShaderManager::UpdateFilePaths()
{
	myFilePaths = Crimson::GetAllFilepathsInDirectory(GetPath(), GetExtension(), true);
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
#ifndef NETWORK_SERVER
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
#else
	shader.myName = ToWString(aPath);
	auto iter = myShaders.emplace(aPath, shader);
	return &iter.first->second;
#endif // !NETWORK_SERVER
}
