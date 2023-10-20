#include "DirectoryFunctions.h"
#include <filesystem>

namespace Crimson
{
	namespace fs = std::filesystem;

	std::unordered_set<std::string> GetFilepathsInDirectory(const std::string& aPath, bool aGetRelativeToAppPath)
	{
		std::unordered_set<std::string> result;
		if (!fs::exists(aPath))
		{
			return result;
		}

		fs::path dirPath;
		if (aGetRelativeToAppPath)
		{
			dirPath = fs::current_path();
		}
		else
		{
			dirPath = aPath;
		}

		for (auto& file : fs::directory_iterator(aPath))
		{
			if (file.path().has_extension())
			{
				const std::string& relativePath = fs::relative(file.path(), dirPath).string();
				result.emplace(relativePath);
			}
		}
		return result;
	}

	std::unordered_set<std::string> GetFilepathsInDirectory(const std::string& aPath, const std::string& anExtension, bool aGetRelativeToAppPath)
	{
		std::unordered_set<std::string> result;
		if (!fs::exists(aPath))
		{
			return result;
		}

		fs::path dirPath;
		if (aGetRelativeToAppPath)
		{
			dirPath = fs::current_path();
		}
		else
		{
			dirPath = aPath;
		}

		for (auto& file : fs::directory_iterator(aPath))
		{
			if (file.path().has_extension() && file.path().extension().string() == anExtension)
			{
				const std::string& relativePath = fs::relative(file.path(), dirPath).string();
				result.emplace(relativePath);
			}
		}
		return result;
	}

	std::unordered_set<std::string> GetFoldersInDirectory(const std::string& aPath, bool aGetRelativeToAppPath)
	{
		std::unordered_set<std::string> result;
		if (!fs::exists(aPath))
		{
			return result;
		}

		fs::path dirPath;
		if (aGetRelativeToAppPath)
		{
			dirPath = fs::current_path();
		}
		else
		{
			dirPath = aPath;
		}

		for (auto& file : fs::directory_iterator(aPath))
		{
			if (file.is_directory())
			{
				result.emplace(fs::relative(file.path(), dirPath).string());
			}
		}
		return result;
	}

	std::unordered_set<std::string> GetAllFilepathsInDirectory(const std::string& aPath, bool aGetRelativeToAppPath)
	{
		std::unordered_set<std::string> result;
		if (!fs::exists(aPath))
		{
			return result;
		}

		fs::path dirPath;
		if (aGetRelativeToAppPath)
		{
			dirPath = fs::current_path();
		}
		else
		{
			dirPath = aPath;
		}

		for (auto& file : fs::recursive_directory_iterator(aPath))
		{
			if (file.path().has_extension())
			{
				const std::string& relativePath = fs::relative(file.path(), dirPath).string();
				result.emplace(relativePath);
			}
		}
		return result;
	}

	std::unordered_set<std::string> GetAllFilepathsInDirectory(const std::string& aPath, const std::string& anExtension, bool aGetRelativeToAppPath)
	{
		std::unordered_set<std::string> result;
		if (!fs::exists(aPath))
		{
			return result;
		}

		fs::path dirPath;
		if (aGetRelativeToAppPath)
		{
			dirPath = fs::current_path();
		}
		else
		{
			dirPath = aPath;
		}

		for (auto& file : fs::recursive_directory_iterator(aPath))
		{
			if (file.path().has_extension() && file.path().extension().string() == anExtension)
			{
				const std::string& relativePath = fs::relative(file.path(), dirPath).string();
				result.emplace(relativePath);
			}
		}
		return result;
	}

	std::unordered_set<std::string> GetAllFoldersInDirectory(const std::string& aPath, bool aGetRelativeToAppPath)
	{
		std::unordered_set<std::string> result;
		if (!fs::exists(aPath))
		{
			return result;
		}

		fs::path dirPath;
		if (aGetRelativeToAppPath)
		{
			dirPath = fs::current_path();
		}
		else
		{
			dirPath = aPath;
		}

		for (auto& file : fs::recursive_directory_iterator(aPath))
		{
			if (file.is_directory())
			{
				result.emplace(fs::relative(file.path(), dirPath).string());
			}
		}
		return result;
	}

	std::unordered_set<std::string> SearchDirectory(const std::string& aPath, const std::string& aSearchWord)
	{
		std::unordered_set<std::string> result;
		if (!fs::exists(aPath))
		{
			return result;
		}

		const auto& appPath = fs::current_path();
		for (auto& file : fs::recursive_directory_iterator(aPath))
		{
			if (file.path().string().find(aSearchWord) != std::string::npos)
			{
				const std::string& relativePath = fs::relative(file.path(), appPath).string();
				result.emplace(relativePath);
			}
		}
		return result;
	}

	std::string GetValidPath(const std::string& aPath, const std::string& aDefaultDirectory)
	{
		std::string fullPath = aPath;
		std::replace(fullPath.begin(), fullPath.end(), '/', '\\');

		if (fs::exists(fullPath))
		{
			return fullPath;
		}

		fullPath = aDefaultDirectory + fullPath;

		if (!fs::exists(fullPath))
		{
			return "";
		}
		return fullPath;
	}

	std::string CreateValidPath(const std::string& aPath, const std::string& aDefaultDirectory)
	{
		std::string fullPath = aPath;
		std::replace(fullPath.begin(), fullPath.end(), '/', '\\');

		if (fs::exists(fullPath))
		{
			return fullPath;
		}

		if (auto firstDivider = fullPath.find_first_of('\\'); !fs::exists(fullPath.substr(0, firstDivider)))
		{
			fullPath = aDefaultDirectory + fullPath;
		}

		fs::path directoryPath = fs::path(fullPath).parent_path();
		if (!fs::create_directories(directoryPath))
		{
			if (!fs::exists(directoryPath))
			{
				return "";
			}
		}
		return fullPath;
	}

	std::string AddExtensionIfMissing(const std::string& aPath, const std::string& anExtension, bool aForceExtension)
	{
		if (aForceExtension && fs::path(aPath).extension() != anExtension)
		{
			return aPath + anExtension;
		}
		else if (fs::path(aPath).has_extension())
		{
			return aPath;
		}
		else
		{
			return aPath + anExtension;
		}
	}

	bool HasValidExtension(const std::string& aPath, const std::string& anExtension)
	{
		if (size_t index = aPath.find_last_of('.'); index != std::string::npos)
		{
			std::string extension = aPath.substr(index);
			if (extension == anExtension)
			{
				return true;
			}
		}

		return false;
	}

	bool HasValidExtension(const std::string& aPath, const std::vector<std::string>& anExtensionList)
	{
		if (size_t index = aPath.find_last_of('.'); index != std::string::npos)
		{
			std::string extension = aPath.substr(index);
			for (auto& ext : anExtensionList)
			{
				if (extension == ext)
				{
					return true;
				}
			}
		}

		return false;
	}

	std::string GetRelativePath(const std::string& aFullPath)
	{
		return fs::relative(aFullPath, fs::current_path()).string();
	}

	std::string GetAbsolutePath(const std::string& aRelativePath)
	{
		return fs::absolute(fs::current_path().string() + "\\" + aRelativePath).string();
	}

	std::string GetContainingFolder(const std::string& aFilePath)
	{
		size_t lastSlash = aFilePath.find_last_of('\\');
		if (lastSlash == std::string::npos)
		{
			lastSlash = aFilePath.find_last_of('/');
			if (lastSlash == std::string::npos)
			{
				return std::string();
			}
		}
		return aFilePath.substr(0, lastSlash + 1);
	}

	std::string GetFileName(const std::string& aFilePath)
	{
		size_t lastSlash = aFilePath.find_last_of('\\');
		if (lastSlash == std::string::npos)
		{
			lastSlash = aFilePath.find_last_of('/');
			if (lastSlash == std::string::npos)
			{
				return aFilePath;
			}
		}
		return aFilePath.substr(lastSlash + 1);
	}

	std::string GetFileNameWithoutExtension(const std::string& aFilePath)
	{
		return RemoveFileExtension(GetFileName(aFilePath));
	}

	std::string GetFileExtension(const std::string& aFilePath)
	{
		size_t lastDot = aFilePath.find_last_of('.');
		if (lastDot == std::string::npos)
		{
			return std::string();
		}
		return aFilePath.substr(lastDot);
	}

	std::string RemoveFileExtension(const std::string& aFilePath)
	{
		size_t lastDot = aFilePath.find_last_of('.');
		return aFilePath.substr(0, lastDot);
	}

	std::string GetAppPath()
	{
		return fs::current_path().string();
	}

	void SetAppPath(const std::string& aPath)
	{
		fs::current_path(aPath);
	}

	bool FileExists(const std::string& aFilePath)
	{
		return fs::exists(aFilePath);
	}

	bool IsFile(const std::string& aPath)
	{
		return fs::path(aPath).has_extension();
	}

	bool IsFolder(const std::string& aPath)
	{
		return !fs::path(aPath).has_extension();
	}
}