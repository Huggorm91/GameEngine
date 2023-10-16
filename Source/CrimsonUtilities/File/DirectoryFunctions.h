#pragma once
#include <unordered_set>
#include "String/StringFunctions.h"

namespace Crimson
{
/**
 * Shows windows file selector.
 * @param outFilePath The full path of the selected file.
 * @param anExtensionFilter Filter limiting the selection of files shown, ex. {L"C++ code files", L"*.cpp;*.h;*.hpp"}.
 * @param aFolder The folder the selector will start in. Needs to be full path!
 * @param aTitle The title of the selector window.
 */
	bool ShowFileSelector(std::string& outFilePath, const std::pair<std::wstring, std::wstring>& anExtensionFilter, const std::wstring& aFolder, const std::wstring& aTitle = L"Select File");

	std::unordered_set<std::string> GetFilepathsInDirectory(const std::string& aPath, bool aGetRelativeToAppPath = false);
	std::unordered_set<std::string> GetFilepathsInDirectory(const std::string& aPath, const std::string& anExtension, bool aGetRelativeToAppPath = false);
	std::unordered_set<std::string> GetFoldersInDirectory(const std::string& aPath, bool aGetRelativeToAppPath = false);

	std::unordered_set<std::string> GetAllFilepathsInDirectory(const std::string& aPath, bool aGetRelativeToAppPath = false);
	std::unordered_set<std::string> GetAllFilepathsInDirectory(const std::string& aPath, const std::string& anExtension, bool aGetRelativeToAppPath = false);
	std::unordered_set<std::string> GetAllFoldersInDirectory(const std::string& aPath, bool aGetRelativeToAppPath = false);

	std::unordered_set<std::string> SearchDirectory(const std::string& aPath, const std::string& aSearchWord);

	std::string GetValidPath(const std::string& aPath, const std::string& aDefaultDirectory);
	std::string CreateValidPath(const std::string& aPath, const std::string& aDefaultDirectory);

	std::string AddExtensionIfMissing(const std::string& aPath, const std::string& anExtension);
	bool HasValidExtension(const std::string& aPath, const std::string& anExtension);
	bool HasValidExtension(const std::string& aPath, const std::vector<std::string>& anExtensionList);

	std::string GetRelativePath(const std::string& aFullPath);
	std::string GetFullPath(const std::string& aRelativePath);

	std::string GetContainingFolder(const std::string& aFilePath);
	std::string GetFileName(const std::string& aFilePath);
	std::string GetFileExtension(const std::string& aFilePath);
	std::string RemoveFileExtension(const std::string& aFilePath);

	std::string GetAppPath();
	void SetAppPath(const std::string& aPath);

	bool FileExists(const std::string& aFilePath);
	bool IsFile(const std::string& aPath);
	bool IsFolder(const std::string& aPath);

	[[nodiscard]] std::string Timestamp();
	[[nodiscard]] std::string FileNameTimestamp();
}