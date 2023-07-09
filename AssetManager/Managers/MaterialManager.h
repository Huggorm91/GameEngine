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
	MaterialManager(const std::string& aPath);
	~MaterialManager() = default;

	void Init();

	Material* GetMaterial(const std::string& aPath);

	Material* CreateMaterial(const Material& anAsset, const std::string& anIdentifier);
	Material* CreateMaterial(const std::string& anIdentifier, Shader* aVertexShader, Shader* aPixelShader, Texture* anAlbedo, Texture* aNormal);

	void SaveMaterial(const Material* aMaterial, const std::string& aPath);

private:
	std::unordered_map<std::string, Material> myMaterials;
	std::unordered_set<std::string> myFilePaths;
	const std::string myPath = "Content/Materials/";

	Material* LoadMaterial(const std::string& aPath);
};