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
	myFilePaths = GetAllFilepathsInDirectory(myPath, GetExtension());
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

#ifdef _DEBUG
	path = GetValidPath(path, myPath + "Debug/", &AMLogger);
#elif _RELEASE
	path = GetValidPath(path, myPath + "Release/", &AMLogger);
#elif _RETAIL
	path = GetValidPath(path, myPath + "Retail/", &AMLogger);
#endif // _DEBUG
	
	if (path.empty())
	{
		AMLogger.Err("ShaderManager: Could not load shader from path: " + aPath);
		return nullptr;
	}

	Shader shader;
	if (RHI::LoadShader(&shader, Helpers::string_cast<std::wstring>(path)))
	{
		auto iter = myShaders.emplace(aPath, shader);
		return &iter.first->second;
	}

	AMLogger.Err("ShaderManager: Could not load a shader from: " + aPath);
	return nullptr;
}
