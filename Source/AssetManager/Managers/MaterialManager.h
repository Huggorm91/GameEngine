#pragma once
#include "GraphicsEngine/Rendering/Shader.h"
#include "GraphicsEngine/Rendering/Texture.h"
#include "GraphicsEngine/Rendering/Material.h"
#include <unordered_map>
#include <unordered_set>

class MaterialManager
{
public:
	MaterialManager() = default;
	~MaterialManager() = default;

	void Init();

	Material* GetMaterial(const std::string& aPath, bool aShouldLogErrors);

	Material* CreateMaterial(const Material& anAsset, const std::string& anIdentifier);
	Material* CreateMaterial(const std::string& anIdentifier, Shader* aVertexShader, Shader* aPixelShader, Texture* anAlbedo, Texture* aNormal);

	void SaveMaterial(const Material* aMaterial, const std::string& aPath);

	static inline const char* GetExtension(){ return ".mat"; }
	static inline const char* GetPath(){ return "..\\Content\\Materials\\"; }
	static inline const wchar_t* GetExtensionW(){ return L".mat"; }
	static inline const wchar_t* GetPathW(){ return L"..\\Content\\Materials\\"; }

private:
	std::unordered_map<std::string, Material> myMaterials;
	std::unordered_set<std::string> myFilePaths;

	Material* LoadMaterial(const std::string& aPath, bool aShouldLogErrors);
};