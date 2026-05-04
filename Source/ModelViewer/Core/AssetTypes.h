#pragma once
#include <vector>

namespace Assets
{
	enum class eAssetType
	{
		Unknown,
		Texture,
		Model,
		AnimatedModel,
		Animation,
		Material,
		Prefab,
		Shader,
		Scene,
		Folder
	};

	std::string GetAssetTypeName(eAssetType aType);
	std::string GetAssetPath(eAssetType aType);

	bool IsType(eAssetType aType, std::string anAsset);

	std::vector<eAssetType> GetPossibleTypes(const std::string& anExtension);
	eAssetType GetModelType(const std::string& aPath);
}