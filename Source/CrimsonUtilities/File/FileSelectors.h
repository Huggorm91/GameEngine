#pragma once
#include "DirectoryFunctions.h"

namespace Crimson
{
	/// <summary> Shows windows open file selector. </summary>
	/// <param name="outFilePath"> The full path of the selected file. </param>
	/// <param name="anExtensionFilter"> Filter limiting the selection of files shown, ex. {L"C++ code files", L"*.cpp;*.h;*.hpp"} </param>
	/// <param name="aFolder"> The folder the selector will start in. Needs to be absolute path! </param>
	/// <param name="aTitle"> The title of the selector window. </param>
	/// <returns> Returns true if successful </returns>
	bool ShowOpenFileSelector(std::string& outFilePath, const std::pair<std::wstring, std::wstring>& anExtensionFilter, const std::wstring& aFolder, const std::wstring& aTitle = L"Open File");

	/// <summary> Shows windows open file selector. </summary>
	/// <param name="outFilePath"> The full path of the selected file. </param>
	/// <param name="aFileName"> The default name for the file. </param>
	/// <param name="anExtension"> Default extension of created file, ex. L"doc;docx" </param>
	/// <param name="anExtensionFilter"> Filter limiting the selection of files shown, ex. {L"C++ code files", L"*.cpp;*.h;*.hpp"} </param>
	/// <param name="aFolder"> The folder the selector will start in. Needs to be absolute path! </param>
	/// <param name="aTitle"> The title of the selector window. </param>
	/// <returns> Returns true if successful </returns>
	bool ShowSaveFileSelector(std::string& outFilePath, const std::wstring& aFileName, const std::wstring& anExtension, const std::pair<std::wstring, std::wstring>& anExtensionFilter, const std::wstring& aFolder, const std::wstring& aTitle = L"Save File");
};