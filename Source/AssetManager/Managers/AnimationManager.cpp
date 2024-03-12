#include "AssetManager.pch.h"
#include "AnimationManager.h"
#include <Importer.h>
#undef LoadAnimation

using namespace Crimson;

void AnimationManager::Init()
{
	UpdateFilePaths();
}

void AnimationManager::UpdateFilePaths()
{
	myFilePaths = GetAllFilepathsInDirectory(GetPath(), GetExtension(), true);
	auto blendspaces = Crimson::GetAllFilepathsInDirectory(GetPath(), GetBlendSpaceExtension(), true);
	for (auto& path : blendspaces)
	{
		myFilePaths.emplace(path);
	}
}

const std::unordered_set<std::string>& AnimationManager::GetAnimationlist()
{
	return myFilePaths;
}

AnimationData* AnimationManager::GetAnimation(const std::string& aPath, bool aShouldLogErrors)
{
	if (auto iter = myAnimationData.find(aPath); iter != myAnimationData.end())
	{
		return &iter->second;
	}
	else
	{
		return LoadAnimation(aPath, aShouldLogErrors);
	}
}

BlendSpace AnimationManager::GetBlendSpace(const std::string& aPath, bool aShouldLogErrors)
{
	if (auto iter = myBlendSpaces.find(aPath); iter != myBlendSpaces.end())
	{
		return iter->second;
	}
	else
	{
		return LoadBlendSpace(aPath, aShouldLogErrors);
	}
}

void AnimationManager::SaveBlendSpace(const BlendSpace& aBlendSpace, const std::string& aPath)
{
	std::string path = aPath;
	if (!HasValidExtension(aPath, GetBlendSpaceExtension()))
	{
		path += GetBlendSpaceExtension();
	}
	path = CreateValidPath(path, GetPath());

	if (path.empty())
	{
		AMLogger.Warn("AnimationManager: Could not save blendspace to path: " + aPath);
		return;
	}

	std::fstream fileStream(path, std::ios::out | std::ios::trunc);
	if (fileStream)
	{
		Json::Value blendspace = aBlendSpace.ToJson();
		blendspace["Path"] = path;
		Json::StreamWriterBuilder builder;
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		writer->write(blendspace, &fileStream);
		fileStream.flush();
	}
	else
	{
		AMLogger.Err("AnimationManager: Could not open file at: " + aPath);
	}
	fileStream.close();
}

AnimationData* AnimationManager::LoadAnimation(const std::string& aPath, bool aShouldLogErrors)
{
	std::string path = AddExtensionIfMissing(aPath, GetExtension());
	path = GetValidPath(path, GetPath());
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
		auto iter = myAnimationData.emplace(aPath, tgaAnimation);
		return &iter.first->second;
	}

	if (aShouldLogErrors)
	{
		AMLogger.Err("AnimationManager: Something went wrong when loading animation at: " + aPath);
	}
	return nullptr;
}

BlendSpace AnimationManager::LoadBlendSpace(const std::string& aPath, bool aShouldLogErrors)
{
	std::string path = Crimson::AddExtensionIfMissing(aPath, GetExtension());
	path = GetValidPath(path, GetPath());
	if (path.empty())
	{
		if (aShouldLogErrors)
		{
			AMLogger.Warn("AnimationManager: Could not load blendspace from path: " + aPath);
		}
		return BlendSpace();
	}

	Json::Value json;
	std::ifstream fileStream(path);
	if (fileStream)
	{
		fileStream >> json;
	}
	else
	{
		if (aShouldLogErrors)
		{
			AMLogger.Err("AnimationManager: Could not open file at: " + aPath);
		}
		fileStream.close();
		return BlendSpace();
	}
	fileStream.close();

	auto iter = myBlendSpaces.emplace(aPath, json);
	return iter.first->second;
}
