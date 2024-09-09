#include "FileLogger.h"
#include <filesystem>
#include "Timestamp.hpp"

FileLogger::FileLogger(const std::string& aFileName): myLogFile()
{
	std::filesystem::path directoryPath = std::filesystem::path(aFileName).parent_path();
	std::filesystem::create_directories(directoryPath);
	myLogFile.open(aFileName, std::ios::trunc);
	if (!myLogFile)
	{
		myLogFile.close();
		throw std::runtime_error("FileLogger: Failed to open file: " + aFileName);
	}
}

FileLogger::~FileLogger()
{
	myLogFile.flush();
	myLogFile.close();
}

void FileLogger::Log(const std::string& aString)
{
	myLogFile << "[" << Timestamp() << "] [   LOG   ] " << aString << std::endl;
}

void FileLogger::Warn(const std::string& aString)
{
	myLogFile << "[" << Timestamp() << "] [ WARNING ] " << aString << std::endl;
}

void FileLogger::Err(const std::string& aString)
{
	myLogFile << "[" << Timestamp() << "] [  ERROR  ] " << aString << std::endl;
}

void FileLogger::Succ(const std::string& aString)
{
	myLogFile << "[" << Timestamp() << "] [ SUCCESS ] " << aString << std::endl;
}

void FileLogger::LogException(const std::exception& anException)
{
	LogException(anException, 0u);
}

void FileLogger::NewLine()
{
	myLogFile << std::endl;
}

void FileLogger::LogException(const std::exception& anException, unsigned aLevel)
{
	myLogFile << "[" << Timestamp() << "] " << std::string(aLevel, ' ') << "[  FATAL  ] " << anException.what() << std::endl;

	try
	{
		std::rethrow_if_nested(anException);
	}
	catch (const std::exception& nestedException)
	{
		LogException(nestedException, aLevel + 1);
	}
	catch (...) {} // Catch all other cases.
}