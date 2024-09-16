#pragma once
#include <unordered_map>
#include <unordered_set>
#ifndef NETWORK_SERVER
#include "GraphicsEngine/Rendering/Material.h"
#else
#include "../Assets/Components/ComponentParts/MeshElement.h"
#endif // !NETWORK_SERVER

class MaterialManager
{
public:
	MaterialManager() = default;
	~MaterialManager() = default;

	void Init();

	void UpdateFilePaths();

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