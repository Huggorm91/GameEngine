#include "Modelviewer.pch.h"
#include "AssetTypes.h"
#include "../Modelviewer.h"

std::string Assets::GetAssetTypeName(eAssetType aType)
{
	switch (aType)
	{
	case Assets::eAssetType::Unknown:
		return "";
	case Assets::eAssetType::Texture:
		return "Texture";
	case Assets::eAssetType::Model:
		return "Model";
	case Assets::eAssetType::AnimatedModel:
		return "AnimatedModel";
	case Assets::eAssetType::Animation:
		return "Animation";
	case Assets::eAssetType::Material:
		return "Material";
	case Assets::eAssetType::Prefab:
		return "Prefab";
	case Assets::eAssetType::Shader:
		return "Shader";
	case Assets::eAssetType::Scene:
		return "Scene";
	case Assets::eAssetType::Folder:
		return "Folder";
	default:
		break;
	}
	return std::string();
}

std::string Assets::GetAssetPath(eAssetType aType)
{
	switch (aType)
	{
	case Assets::eAssetType::Unknown:
	case Assets::eAssetType::Folder:
	{
		return "Content\\";
		break;
	}
	case Assets::eAssetType::Texture:
	{
		return AssetManager::GetTexturePath();
		break;
	}
	case Assets::eAssetType::Model:
	case Assets::eAssetType::AnimatedModel:
	{
		return AssetManager::GetModelPath();
		break;
	}
	case Assets::eAssetType::Animation:
	{
		return AssetManager::GetAnimationPath();
		break;
	}
	case Assets::eAssetType::Material:
	{
		return AssetManager::GetMaterialPath();
		break;
	}
	case Assets::eAssetType::Prefab:
	{
		return AssetManager::GetPrefabPath();
		break;
	}
	case Assets::eAssetType::Shader:
	{
		return AssetManager::GetShaderPath();
		break;
	}
	case Assets::eAssetType::Scene:
	{
		return AssetManager::GetScenePath();
		break;
	}
	default:
		break;
	}
	ModelViewer::GetLogger().Err("Assets: Invalid Type! Failed to get AssetPath!");
	return std::string();
}

bool Assets::IsType(eAssetType aType, std::string anAsset)
{
	std::string extension = Crimson::ToLower(Crimson::GetFileExtension(anAsset));

	switch (aType)
	{
	case Assets::eAssetType::Unknown:
	{
		return true;
	}
	case Assets::eAssetType::Texture:
	{
		if (extension == AssetManager::GetTextureExtension())
		{
			return true;
		}
		break;
	}
	case Assets::eAssetType::Model:
	case Assets::eAssetType::AnimatedModel:
	{
		if (extension == AssetManager::GetModelExtension())
		{
			return true;
		}
		break;
	}
	case Assets::eAssetType::Animation:
	{
		if (extension == AssetManager::GetAnimationExtension())
		{
			return true;
		}
		break;
	}
	case Assets::eAssetType::Material:
	{
		if (extension == AssetManager::GetMaterialExtension())
		{
			return true;
		}
		break;
	}
	case Assets::eAssetType::Prefab:
	{
		if (extension == AssetManager::GetPrefabExtension())
		{
			return true;
		}
		break;
	}
	case Assets::eAssetType::Shader:
	{
		if (extension == AssetManager::GetShaderExtension())
		{
			return true;
		}
		break;
	}
	case Assets::eAssetType::Scene:
	{
		if (extension == AssetManager::GetSceneExtension() || extension == AssetManager::GetSceneBinaryExtension())
		{
			return true;
		}
		break;
	}
	case Assets::eAssetType::Folder:
	{
		if (extension.empty())
		{
			return true;
		}
		break;
	}
	default:
		break;
	}

	return false;
}

std::vector<Assets::eAssetType> Assets::GetPossibleTypes(const std::string& anExtension)
{
	std::vector<eAssetType> result;
	std::string extension = Crimson::ToLower(anExtension);
	if (extension.empty())
	{
		result.emplace_back(eAssetType::Folder);
		return result;
	}

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

	if (extension == AssetManager::GetSceneExtension() || extension == AssetManager::GetSceneBinaryExtension())
	{
		result.emplace_back(eAssetType::Scene);
	}


	if (result.size() == 0)
	{
		result.emplace_back(eAssetType::Unknown);
	}

	return result;
}

Assets::eAssetType Assets::GetModelType(const std::string& aPath)
{
	eAssetType result = eAssetType::Unknown;
	AssetManager::SetLogErrors(false);
	if (AssetManager::GetAsset<Animation>(aPath).IsValid())
	{
		result = eAssetType::Animation;
	}
	else
	{
		if (AssetManager::GetAsset<GameObject>(aPath).HasComponent<AnimatedMeshComponent>())
		{
			result = eAssetType::AnimatedModel;
		}
		else
		{
			result = eAssetType::Model;
		}
	}
	AssetManager::SetLogErrors(true);
	return result;
}
