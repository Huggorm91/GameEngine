#include "AssetManager.pch.h"
#include "AnimationManager.h"
#include "../DirectoryFunctions.h"
#include <Importer.h>
#undef LoadAnimation

AnimationManager::AnimationManager(const std::string& aPath): myPath(aPath), myAnimationData(), myAnimations(), myFilePaths()
{
}

void AnimationManager::Init()
{
	myFilePaths = GetAllFilepathsInDirectory(myPath);
}

Animation* AnimationManager::GetAnimation(const std::string& aPath)
{
	if (auto iter = myAnimations.find(aPath); iter != myAnimations.end())
	{
		return &iter->second;
	}
	else
	{
		return LoadAnimation(aPath);
	}
}

Animation* AnimationManager::LoadAnimation(const std::string& aPath)
{
	TGA::FBX::Animation tgaAnimation;
	bool success = false;
	try
	{
		success = TGA::FBX::Importer::LoadAnimationA(aPath, tgaAnimation);
	}
	catch (const std::exception& e)
	{
		AMLogger.Err("AnimationManager: Could not load animation from: " + aPath);
		AMLogger.LogException(e);
	}

	if (success)
	{
		auto dataIter = myAnimationData.emplace(aPath, tgaAnimation);
		auto iter = myAnimations.emplace(aPath, dataIter.first->second);
		return &iter.first->second;
	}
	AMLogger.Err("AnimationManager: Something went wrong when loading animation at: " + aPath);
	return nullptr;
}
