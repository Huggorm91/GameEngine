#pragma once
#include "GraphicsEngine/Rendering/Shader.h"
#include <unordered_map>
#include <unordered_set>

class ShaderManager
{
public:
	ShaderManager() = default;
	~ShaderManager() = default;

	void Init();

	Shader* GetShader(const std::string& aPath);

	static inline const char* GetExtension(){ return ".cso"; }
	static inline const char* GetPath()
	{ 
#ifdef _DEBUG
		return "Content\\Shaders\\Debug\\";
#elif _RELEASE
		return "Content\\Shaders\\Release\\";
#elif _RETAIL
		return "Content\\Shaders\\Retail\\";
#endif // _DEBUG 
	}
	static inline const wchar_t* GetExtensionW(){ return L".cso"; }
	static inline const wchar_t* GetPathW()
	{ 
#ifdef _DEBUG
		return L"Content\\Shaders\\Debug\\";
#elif _RELEASE
		return L"Content\\Shaders\\Release\\";
#elif _RETAIL
		return L"Content\\Shaders\\Retail\\";
#endif // _DEBUG 
	}

private:
	std::unordered_map<std::string, Shader> myShaders;
	std::unordered_set<std::string> myFilePaths;

	Shader* LoadShader(const std::string& aPath);
};