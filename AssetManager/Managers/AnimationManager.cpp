#include "AssetManager.pch.h"
#include "AnimationManager.h"
#include "File/DirectoryFunctions.h"
#include <Importer.h>
#undef LoadAnimation

void AnimationManager::Init()
{
	myFilePaths = Crimson::GetAllFilepathsInDirectory(GetPath(), GetExtension());
}

Animation* AnimationManager::GetAnimation(const std::string& aPath, bool aShouldLogErrors)
{
	if (auto iter = myAnimations.find(aPath); iter != myAnimations.end())
	{
		return &iter->second;
	}
	else
	{
		return LoadAnimation(aPath, aShouldLogErrors);
	}
}

Animation* AnimationManager::LoadAnimation(const std::string& aPath, bool aShouldLogErrors)
{
	std::string path = Crimson::AddExtensionIfMissing(aPath, GetExtension());
	path = Crimson::GetValidPath(path, GetPath());
	if (path.empty())
	{
		if (aShouldLogErrors)
		{
			AMLogger.Warn("AnimationManager: Could not load animation from path: " + aPath);
		}
		return nullptr;
	}

	TGA::FBX::Animation tgaAnimation;
	bool success = false;
	try
	{
		success = TGA::FBX::Importer::LoadAnimationA(path, tgaAnimation);
	}
	catch (const std::exception& e)
	{
		if (aShouldLogErrors)
		{
			AMLogger.Warn("AnimationManager: Could not load animation from: " + aPath);
			AMLogger.Err(e.what());
		}
		return nullptr;
	}

	if (success)
	{
		auto dataIter = myAnimationData.emplace(aPath, tgaAnimation);
		dataIter.first->second.myPath = &dataIter.first->first;
		auto iter = myAnimations.emplace(aPath, dataIter.first->second);
		return &iter.first->second;
	}

	if (aShouldLogErrors)
	{
		AMLogger.Err("AnimationManager: Something went wrong when loading animation at: " + aPath);
	}
	return nullptr;
}
