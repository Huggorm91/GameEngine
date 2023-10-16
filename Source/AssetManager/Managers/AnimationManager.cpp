#include "AssetManager.pch.h"
#include "AnimationManager.h"
#include <Importer.h>
#undef LoadAnimation

using namespace Crimson;

void AnimationManager::Init()
{
	myFilePaths = GetAllFilepathsInDirectory(GetPath(), GetExtension());
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
	std::string path = AddExtensionIfMissing(aPath, GetExtension());
	path = GetValidPath(path, GetPath());
	if (path.empty())
	{
		AMLogger.Err("AnimationManager: Could not load animation from path: " + aPath);
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
		AMLogger.Err("AnimationManager: Could not load animation from: " + aPath);
		AMLogger.LogException(e);
	}

	if (success)
	{
		auto dataIter = myAnimationData.emplace(aPath, tgaAnimation);
		dataIter.first->second.myPath = &dataIter.first->first;
		auto iter = myAnimations.emplace(aPath, dataIter.first->second);
		return &iter.first->second;
	}

	AMLogger.Err("AnimationManager: Something went wrong when loading animation at: " + aPath);
	return nullptr;
}
