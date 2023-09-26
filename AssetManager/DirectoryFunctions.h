#pragma once
#include <unordered_set>

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

std::string GetValidPath(const std::string& aPath, const std::string& aDefaultDirectory, const Logger* aLogger = nullptr);
std::string CreateValidPath(const std::string& aPath, const std::string& aDefaultDirectory, const Logger* aLogger = nullptr);

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

std::string RemoveStringPart(const std::string& aString, const std::string& aPartToRemove);

std::wstring ToWString(const std::string& aString);
std::string ToString(const std::wstring& aString);

std::string ToLower(const std::string& aString);
std::string ToUpper(const std::string& aString);