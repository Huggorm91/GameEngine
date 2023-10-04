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

	std::unordered_set<std::string> GetAllFilepathsInDirectory(const std::string& aPath);
	std::unordered_set<std::string> GetAllFilepathsInDirectory(const std::string& aPath, const std::string& anExtension);
	std::unordered_set<std::string> GetAllFoldersInDirectory(const std::string& aPath);

	std::string GetValidPath(const std::string& aPath, const std::string& aDefaultDirectory);
	std::string CreateValidPath(const std::string& aPath, const std::string& aDefaultDirectory);

	std::string AddExtensionIfMissing(const std::string& aPath, const std::string& anExtension);
	bool HasValidExtension(const std::string& aPath, const std::string& anExtension);

	std::string GetRelativePath(const std::string& aFullPath);
	std::string GetFullPath(const std::string& aRelativePath);

	std::string GetContainingFolder(const std::string& aFilePath);
	std::string GetFileName(const std::string& aFilePath);
	std::string GetFileExtension(const std::string& aFilePath);
	std::string RemoveFileExtension(const std::string& aFilePath);

	std::string GetAppPath();

	bool FileExists(const std::string& aFilePath);

	[[nodiscard]] std::string Timestamp();
	[[nodiscard]] std::string FileNameTimestamp();
}