#include "FileSelectors.h"
#include <ShObjIdl.h>

namespace Crimson
{
	bool ShowOpenFileSelector(std::string& outFilePath, const std::pair<std::wstring, std::wstring>& anExtensionFilter, const std::wstring& aFolder, const std::wstring& aTitle)
	{
		// https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/nn-shobjidl_core-ifiledialog

		HRESULT sysHR = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (FAILED(sysHR))
		{
			return false;
		}

		IFileOpenDialog* fileSystem = nullptr;
		sysHR = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&fileSystem));
		if (FAILED(sysHR)) {
			CoUninitialize();
			return false;
		}

		// https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/ne-shobjidl_core-_fileopendialogoptions
		DWORD options;
		fileSystem->GetOptions(&options);
		fileSystem->SetOptions(options | FOS_HIDEPINNEDPLACES);

		// Example for multiple filters
		//COMDLG_FILTERSPEC ComDlgFS[3] = { {L"C++ code files", L"*.cpp;*.h;*.rc"},{L"Executable Files", L"*.exe;*.dll"}, {L"All Files",L"*.*"} };
		//pFileOpen->SetFileTypes(3, ComDlgFS);
		COMDLG_FILTERSPEC ComDlgFS{ anExtensionFilter.first.c_str(), anExtensionFilter.second.c_str() };
		fileSystem->SetFileTypes(1, &ComDlgFS);

		fileSystem->SetTitle(aTitle.c_str());

		IShellItem* folder = NULL;
		sysHR = SHCreateItemFromParsingName(aFolder.c_str(), NULL, IID_PPV_ARGS(&folder));
		if (SUCCEEDED(sysHR))
		{
			// Only forces folder first time
			//fileSystem->SetDefaultFolder(folder);

			// Forces folder every time
			fileSystem->SetFolder(folder);
			folder->Release();
		}

		sysHR = fileSystem->Show(NULL);
		if (FAILED(sysHR)) {
			fileSystem->Release();
			CoUninitialize();
			return false;
		}

		IShellItem* files;
		sysHR = fileSystem->GetResult(&files);
		if (FAILED(sysHR)) {
			fileSystem->Release();
			CoUninitialize();
			return false;
		}

		PWSTR path;
		sysHR = files->GetDisplayName(SIGDN_FILESYSPATH, &path);
		if (FAILED(sysHR)) {
			files->Release();
			fileSystem->Release();
			CoUninitialize();
			return false;
		}

		outFilePath = ToString(path);

		CoTaskMemFree(path);
		files->Release();
		fileSystem->Release();
		CoUninitialize();
		return true;
	}

	bool ShowSaveFileSelector(std::string& outFilePath, const std::wstring& aFileName, const std::wstring& anExtension, const std::pair<std::wstring, std::wstring>& anExtensionFilter, const std::wstring& aFolder, const std::wstring& aTitle)
	{
		// https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/nn-shobjidl_core-ifiledialog

		HRESULT sysHR = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (FAILED(sysHR))
		{
			return false;
		}

		IFileSaveDialog* fileSystem = nullptr;
		sysHR = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&fileSystem));
		if (FAILED(sysHR)) {
			CoUninitialize();
			return false;
		}

		// https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/ne-shobjidl_core-_fileopendialogoptions
		DWORD options;
		fileSystem->GetOptions(&options);
		fileSystem->SetOptions(options | FOS_HIDEPINNEDPLACES);

		// Example for multiple filters
		//COMDLG_FILTERSPEC ComDlgFS[3] = { {L"C++ code files", L"*.cpp;*.h;*.rc"},{L"Executable Files", L"*.exe;*.dll"}, {L"All Files",L"*.*"} };
		//pFileOpen->SetFileTypes(3, ComDlgFS);
		COMDLG_FILTERSPEC ComDlgFS{ anExtensionFilter.first.c_str(), anExtensionFilter.second.c_str() };
		fileSystem->SetFileTypes(1, &ComDlgFS);

		fileSystem->SetDefaultExtension(anExtension.c_str());

		fileSystem->SetTitle(aTitle.c_str());

		fileSystem->SetFileName(aFileName.c_str());

		IShellItem* folder = NULL;
		sysHR = SHCreateItemFromParsingName(aFolder.c_str(), NULL, IID_PPV_ARGS(&folder));
		if (SUCCEEDED(sysHR))
		{
			// Only forces folder first time
			//fileSystem->SetDefaultFolder(folder);

			// Forces folder every time
			sysHR = fileSystem->SetFolder(folder);
			folder->Release();
			folder = NULL;
		}

		sysHR = fileSystem->Show(NULL);
		if (FAILED(sysHR)) {
			fileSystem->Release();
			CoUninitialize();
			return false;
		}

		IShellItem* files;
		sysHR = fileSystem->GetResult(&files);
		if (FAILED(sysHR)) {
			fileSystem->Release();
			CoUninitialize();
			return false;
		}

		PWSTR path;
		sysHR = files->GetDisplayName(SIGDN_FILESYSPATH, &path);
		if (FAILED(sysHR)) {
			files->Release();
			fileSystem->Release();
			CoUninitialize();
			return false;
		}

		outFilePath = ToString(path);

		CoTaskMemFree(path);
		files->Release();
		fileSystem->Release();
		CoUninitialize();
		return true;
	}
}