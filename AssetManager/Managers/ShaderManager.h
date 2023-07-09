#pragma once
#include "GraphicsEngine/Rendering/Shader.h"
#include <unordered_map>
#include <unordered_set>

class ShaderManager
{
public:
	ShaderManager() = default;
	ShaderManager(const std::string& aPath);
	~ShaderManager() = default;

	void Init();

	Shader* GetShader(const std::string& aPath);

private:
	std::unordered_map<std::string, Shader> myShaders;
	std::unordered_set<std::string> myFilePaths;
	const std::string myPath = "Content/Shaders/";

	Shader* LoadShader(const std::string& aPath);
};