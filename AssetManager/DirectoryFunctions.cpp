#include "AssetManager.pch.h"
#include "DirectoryFunctions.h"
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

std::unordered_set<std::string> GetAllFilepathsInDirectory(const std::string& aPath)
{
	std::unordered_set<std::string> result{};
	const std::string& appPath = fs::current_path().string();

	for (auto& file : fs::recursive_directory_iterator(aPath))
	{
		const std::string& fullFilename = file.path().filename().string();
		
		if (file.path().has_extension())
		{
			const std::string& relativePath = fs::relative(file.path(), appPath).string();
			result.emplace(relativePath);
		}
	}
	return result;
}

std::string GetValidPath(const std::string& aPath, const std::string& aDefaultDirectory)
{
	if (fs::exists(aPath))
	{
		return aPath;
	}
	std::string fullPath = aDefaultDirectory + aPath;

	if (!fs::exists(fullPath))
	{
		AMLogger.Err("Incorrect path: " + aPath);
		return "";
	}
	return fullPath;
}

std::string CreateValidPath(const std::string& aPath, const std::string& aDefaultDirectory)
{
	if (fs::exists(aPath))
	{
		return aPath;
	}
	std::string fullPath = aPath;
	std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

	if (auto firstDivider = fullPath.find_first_of('/'); !fs::exists(fullPath.substr(0, firstDivider)))
	{
		fullPath = aDefaultDirectory + fullPath;
	}

	fs::path directoryPath = fs::path(fullPath).parent_path();
	if (!fs::create_directories(directoryPath))
	{
		if (!fs::exists(directoryPath))
		{
			AMLogger.Err("Failed to create directories for path: " + aPath);
			return "";
		}
	}
    return fullPath;
}

std::string AddExtensionIfMissing(const std::string& aPath, const std::string& anExtension)
{
	fs::path path = aPath;
	if (path.has_extension())
	{
		return aPath;
	}
	else
	{
		return aPath + anExtension;
	}
}
