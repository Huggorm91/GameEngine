#include "DirectoryFunctions.h"
#include <filesystem>
#include <ShObjIdl.h>
#include <assert.h>

namespace fs = std::filesystem;

bool ShowFileSelector(std::string& outFilePath, const std::pair<std::wstring, std::wstring>& anExtensionFilter, const std::wstring& aFolder, const std::wstring& aTitle)
{
	// https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/nn-shobjidl_core-ifiledialog

	HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(f_SysHr))
	{
		return FALSE;
	}

	IFileOpenDialog* f_FileSystem;
	f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
	if (FAILED(f_SysHr)) {
		CoUninitialize();
		return FALSE;
	}

	// https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/ne-shobjidl_core-_fileopendialogoptions
	f_FileSystem->SetOptions(FOS_HIDEPINNEDPLACES);

	// Example for multiple filters
	//COMDLG_FILTERSPEC ComDlgFS[3] = { {L"C++ code files", L"*.cpp;*.h;*.rc"},{L"Executable Files", L"*.exe;*.dll"}, {L"All Files",L"*.*"} };
	//pFileOpen->SetFileTypes(3, ComDlgFS);
	COMDLG_FILTERSPEC ComDlgFS{ anExtensionFilter.first.c_str(), anExtensionFilter.second.c_str() };
	f_FileSystem->SetFileTypes(1, &ComDlgFS);

	f_FileSystem->SetTitle(aTitle.c_str());

	IShellItem* folder = NULL;
	f_SysHr = SHCreateItemFromParsingName(aFolder.c_str(), NULL, IID_PPV_ARGS(&folder));
	if (SUCCEEDED(f_SysHr))
	{
		// Only forces folder first time
		//f_FileSystem->SetDefaultFolder(folder);

		// Forces folder every time
		f_FileSystem->SetFolder(folder);
		folder->Release();
	}

	f_SysHr = f_FileSystem->Show(NULL);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	IShellItem* f_Files;
	f_SysHr = f_FileSystem->GetResult(&f_Files);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	PWSTR f_Path;
	f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
	if (FAILED(f_SysHr)) {
		f_Files->Release();
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	outFilePath = ToString(f_Path);

	CoTaskMemFree(f_Path);
	f_Files->Release();
	f_FileSystem->Release();
	CoUninitialize();
	return TRUE;
}

std::unordered_set<std::string> GetAllFilepathsInDirectory(const std::string& aPath)
{
	std::unordered_set<std::string> result{};
	const std::string& appPath = fs::current_path().string();
	fs::create_directories(aPath);

	for (auto& file : fs::recursive_directory_iterator(aPath))
	{
		if (file.path().has_extension())
		{
			const std::string& relativePath = fs::relative(file.path(), appPath).string();
			result.emplace(relativePath);
		}
	}
	return result;
}

std::unordered_set<std::string> GetAllFilepathsInDirectory(const std::string& aPath, const std::string& anExtension)
{
	std::unordered_set<std::string> result{};
	const std::string& appPath = fs::current_path().string();
	fs::create_directories(aPath);

	for (auto& file : fs::recursive_directory_iterator(aPath))
	{
		if (file.path().has_extension() && file.path().extension().string() == anExtension)
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

bool HasValidExtension(const std::string& aPath, const std::string& anExtension)
{
	size_t index = aPath.find_last_of('.');
	if (index != std::string::npos)
	{
		std::string extension = aPath.substr(index);
		if (extension == anExtension)
		{
			return true;
		}
	}

	return false;
}

std::string GetRelativePath(const std::string& aFullPath)
{
	const std::string& appPath = fs::current_path().string();
	return fs::relative(aFullPath, appPath).string();
}

std::string GetFullPath(const std::string& aRelativePath)
{
	return fs::current_path().string() + "\\" + aRelativePath;
}

std::string GetContainingFolder(const std::string& aFilePath)
{
	size_t lastSlash = aFilePath.find_last_of('\\');
	if (lastSlash == std::string::npos)
	{
		lastSlash = aFilePath.find_last_of('/');
	}
	return aFilePath.substr(0, lastSlash + 1);
}

std::string GetFileName(const std::string& aFilePath)
{
	size_t lastSlash = aFilePath.find_last_of('\\');
	if (lastSlash == std::string::npos)
	{
		lastSlash = aFilePath.find_last_of('/');
	}
	return aFilePath.substr(lastSlash + 1);
}

std::string GetFileExtension(const std::string& aFilePath)
{
	size_t lastSlash = aFilePath.find_last_of('.');
	return aFilePath.substr(lastSlash);
}

std::string RemoveFileExtension(const std::string& aFilePath)
{
	size_t lastSlash = aFilePath.find_last_of('.');
	return aFilePath.substr(0, lastSlash);
}

std::string GetAppPath()
{
	return fs::current_path().string();
}

bool FileExists(const std::string& aFilePath)
{
	return fs::exists(aFilePath);
}

std::string RemoveStringPart(const std::string& aString, const std::string& aPartToRemove)
{
	assert(aString.size() >= aPartToRemove.size() && "Trying to remove too much!");
	auto index = aString.find(aPartToRemove);
	if (index == std::string::npos)
	{
		return aString;
	}

	std::string result(aString);
	result.erase(index, aPartToRemove.size());
	return result;
}

std::wstring ToWString(const std::string& aString)
{
	const int sLength = static_cast<int>(aString.length());
	const int len = MultiByteToWideChar(CP_ACP, 0, aString.c_str(), sLength, 0, 0);
	std::wstring result(len, L'\0');
	MultiByteToWideChar(CP_ACP, 0, aString.c_str(), sLength, &result[0], len);
	return result;
}

std::string ToString(const std::wstring& aString)
{
	const int sLength = static_cast<int>(aString.length());
	const int len = WideCharToMultiByte(CP_ACP, 0, aString.c_str(), sLength, 0, 0, 0, 0);
	std::string result(len, L'\0');
	WideCharToMultiByte(CP_ACP, 0, aString.c_str(), sLength, &result[0], len, 0, 0);
	return result;
}

std::string ToLower(const std::string& aString)
{
	std::string lowerCase = aString;
	std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
	return lowerCase;
}

std::string ToUpper(const std::string& aString)
{
	std::string upperCase = aString;
	std::transform(upperCase.begin(), upperCase.end(), upperCase.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
	return upperCase;
}
