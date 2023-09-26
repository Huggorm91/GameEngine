#pragma once
#include <vector>

namespace AssetTypes
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
	};

	std::string GetAssetTypeName(eAssetType aType);
	std::string GetAssetPath(eAssetType aType);

	std::vector<eAssetType> GetPossibleTypes(const std::string& anExtension);
}