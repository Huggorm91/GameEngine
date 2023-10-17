#pragma once
#include <unordered_set>
#include "String/StringFunctions.h"

namespace Crimson
{
	std::unordered_set<std::string> GetFilepathsInDirectory(const std::string& aPath, bool aGetRelativeToAppPath = false);
	std::unordered_set<std::string> GetFilepathsInDirectory(const std::string& aPath, const std::string& anExtension, bool aGetRelativeToAppPath = false);
	std::unordered_set<std::string> GetFoldersInDirectory(const std::string& aPath, bool aGetRelativeToAppPath = false);

	std::unordered_set<std::string> GetAllFilepathsInDirectory(const std::string& aPath, bool aGetRelativeToAppPath = false);
	std::unordered_set<std::string> GetAllFilepathsInDirectory(const std::string& aPath, const std::string& anExtension, bool aGetRelativeToAppPath = false);
	std::unordered_set<std::string> GetAllFoldersInDirectory(const std::string& aPath, bool aGetRelativeToAppPath = false);

	std::unordered_set<std::string> SearchDirectory(const std::string& aPath, const std::string& aSearchWord);

	std::string GetValidPath(const std::string& aPath, const std::string& aDefaultDirectory);
	std::string CreateValidPath(const std::string& aPath, const std::string& aDefaultDirectory);

	std::string AddExtensionIfMissing(const std::string& aPath, const std::string& anExtension, bool aForceExtension = false);
	bool HasValidExtension(const std::string& aPath, const std::string& anExtension);
	bool HasValidExtension(const std::string& aPath, const std::vector<std::string>& anExtensionList);

	std::string GetRelativePath(const std::string& aFullPath);
	std::string GetAbsolutePath(const std::string& aRelativePath);

	std::string GetContainingFolder(const std::string& aFilePath);
	std::string GetFileName(const std::string& aFilePath);
	std::string GetFileNameWithoutExtension(const std::string& aFilePath);
	std::string GetFileExtension(const std::string& aFilePath);
	std::string RemoveFileExtension(const std::string& aFilePath);

	std::string GetAppPath();
	void SetAppPath(const std::string& aPath);

	bool FileExists(const std::string& aFilePath);
	bool IsFile(const std::string& aPath);
	bool IsFolder(const std::string& aPath);
}