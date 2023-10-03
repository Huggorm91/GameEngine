#include "AssetManager.pch.h"
#include "AssetTypes.h"
#include "AssetManager/AssetManager.h"
#include "File/DirectoryFunctions.h"
#include "../Modelviewer.h"

std::string AssetTypes::GetAssetTypeName(eAssetType aType)
{
	switch (aType)
	{
	case AssetTypes::eAssetType::Unknown:
		return "";
	case AssetTypes::eAssetType::Texture:
		return "Texture";
	case AssetTypes::eAssetType::Model:
		return "Model";
	case AssetTypes::eAssetType::AnimatedModel:
		return "AnimatedModel";
	case AssetTypes::eAssetType::Animation:
		return "Animation";
	case AssetTypes::eAssetType::Material:
		return "Material";
	case AssetTypes::eAssetType::Prefab:
		return "Prefab";
	case AssetTypes::eAssetType::Shader:
		return "Shader";
	case AssetTypes::eAssetType::Scene:
		return "Scene";
	default:
		break;
	}
	return std::string();
}

std::string AssetTypes::GetAssetPath(eAssetType aType)
{
	switch (aType)
	{
	case AssetTypes::eAssetType::Unknown:
	{
		return "Content\\";
		break;
	}
	case AssetTypes::eAssetType::Texture:
	{
		return AssetManager::GetTexturePath();
		break;
	}
	case AssetTypes::eAssetType::Model:
	case AssetTypes::eAssetType::AnimatedModel:
	{
		return AssetManager::GetModelPath();
		break;
	}
	case AssetTypes::eAssetType::Animation:
	{
		return AssetManager::GetAnimationPath();
		break;
	}
	case AssetTypes::eAssetType::Material:
	{
		return AssetManager::GetMaterialPath();
		break;
	}
	case AssetTypes::eAssetType::Prefab:
	{
		return AssetManager::GetPrefabPath();
		break;
	}
	case AssetTypes::eAssetType::Shader:
	{
		return AssetManager::GetShaderPath();
		break;
	}
	case AssetTypes::eAssetType::Scene:
	{
		return ModelViewer::GetScenePath();
		break;
	}
	default:
		break;
	}
	ModelViewer::GetLogger().Err("AssetTypes: Failed to get AssetPath!");
	return std::string();
}

std::vector<AssetTypes::eAssetType> AssetTypes::GetPossibleTypes(const std::string& anExtension)
{
	std::vector<eAssetType> result;
	std::string extension = ToLower(anExtension);
	if (extension == AssetManager::GetModelExtension())
	{
		result.emplace_back(eAssetType::Model);
		result.emplace_back(eAssetType::AnimatedModel);
	}

	if (extension == AssetManager::GetAnimationExtension())
	{
		result.emplace_back(eAssetType::Animation);
	}

	if (extension == AssetManager::GetMaterialExtension())
	{
		result.emplace_back(eAssetType::Material);
	}

	if (extension == AssetManager::GetPrefabExtension())
	{
		result.emplace_back(eAssetType::Prefab);
	}

	if (extension == AssetManager::GetShaderExtension())
	{
		result.emplace_back(eAssetType::Shader);
	}

	if (extension == AssetManager::GetTextureExtension())
	{
		result.emplace_back(eAssetType::Texture);
	}

	if (extension == ModelViewer::GetSceneExtension())
	{
		result.emplace_back(eAssetType::Scene);
	}


	if (result.size() == 0)
	{
		result.emplace_back(eAssetType::Unknown);
	}

	return result;
}
