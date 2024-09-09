#include "MainLogger.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include "Timestamp.hpp"

MainLogger::MainLogger() : shouldPrintToOutput(false), myHandle(GetConsoleWindow())
{
}

void MainLogger::PrintHistoryToFile(const std::string& aFileName)
{
	std::filesystem::path directoryPath = std::filesystem::path(aFileName).parent_path();
	std::filesystem::create_directories(directoryPath);
	std::ofstream fileStream(aFileName, std::ios::app);
	if (fileStream)
	{
		fileStream << myHistory.str();
		fileStream.flush();
	}
}

void MainLogger::SetPrintToVSOutput(bool aNewValue)
{
	shouldPrintToOutput = aNewValue;
}

void MainLogger::Log(const std::string& aString)
{
	const std::string& timestamp = Timestamp();
	std::string message = "[" + timestamp + "] [   LOG   ] " + aString;
	myHistory << message + "\n";

#ifndef _RETAIL
	if (shouldPrintToOutput)
	{
		OutputDebugStringA(message.c_str());
	}
	else
	{
		std::unique_lock lock(myCoutMutex);
		SetConsoleTextAttribute(myHandle, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		std::cout << "[" << timestamp << "] [   LOG   ] ";
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		std::cout << aString << std::endl;
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
#endif // !_RETAIL
}

void MainLogger::Log(const std::string& aString, const std::string& aNameSpace)
{
	const std::string& timestamp = Timestamp();
	std::string message = "[" + timestamp + "] [ " + aNameSpace + " ] [   LOG   ] " + aString;
	myHistory << message + "\n";

#ifndef _RETAIL
	if (shouldPrintToOutput)
	{
		OutputDebugStringA(message.c_str());
	}
	else
	{
		std::unique_lock lock(myCoutMutex);
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "[" << timestamp << "] [ " << aNameSpace << " ] [   LOG   ] ";
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		std::cout << aString << std::endl;
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
#endif // !_RETAIL
}

void MainLogger::Warn(const std::string& aString)
{
	const std::string& timestamp = Timestamp();
	std::string message = "[" + timestamp + "] [ WARNING ] " + aString;
	myHistory << message + "\n";

#ifndef _RETAIL
	if (shouldPrintToOutput)
	{
		OutputDebugStringA(message.c_str());
	}
	else
	{
		std::unique_lock lock(myCoutMutex);
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "[" << timestamp << "]";
		SetConsoleTextAttribute(myHandle, BACKGROUND_RED | BACKGROUND_GREEN);
		std::cout << " [ WARNING ] ";
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << aString << std::endl;
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
#endif // !_RETAIL
}

void MainLogger::Warn(const std::string& aString, const std::string& aNameSpace)
{
	const std::string& timestamp = Timestamp();
	std::string message = "[" + timestamp + "] [ " + aNameSpace + " ] [ WARNING ] " + aString;
	myHistory << message + "\n";

#ifndef _RETAIL
	if (shouldPrintToOutput)
	{
		OutputDebugStringA(message.c_str());
	}
	else
	{
		std::unique_lock lock(myCoutMutex);
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "[" << timestamp << "] [ " << aNameSpace << " ]";
		SetConsoleTextAttribute(myHandle, BACKGROUND_RED | BACKGROUND_GREEN);
		std::cout << " [ WARNING ] ";
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << aString << std::endl;
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
#endif // !_RETAIL
}

void MainLogger::Err(const std::string& aString)
{
	const std::string& timestamp = Timestamp();
	std::string message = "[" + timestamp + "] [  ERROR  ] " + aString;
	myHistory << message + "\n";

#ifndef _RETAIL
	if (shouldPrintToOutput)
	{
		OutputDebugStringA(message.c_str());
	}
	else
	{
		std::unique_lock lock(myCoutMutex);
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "[" << timestamp << "]";
		SetConsoleTextAttribute(myHandle, BACKGROUND_RED);
		std::cout << " [  ERROR  ] ";
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << aString << std::endl;
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
#endif // !_RETAIL
}

void MainLogger::Err(const std::string& aString, const std::string& aNameSpace)
{
	const std::string& timestamp = Timestamp();
	std::string message = "[" + timestamp + "] [ " + aNameSpace + " ] [  ERROR  ] " + aString;
	myHistory << message + "\n";

#ifndef _RETAIL
	if (shouldPrintToOutput)
	{
		OutputDebugStringA(message.c_str());
	}
	else
	{
		std::unique_lock lock(myCoutMutex);
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "[" << timestamp << "] [ " << aNameSpace << " ]";
		SetConsoleTextAttribute(myHandle, BACKGROUND_RED);
		std::cout << " [  ERROR  ] ";
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << aString << std::endl;
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
#endif // !_RETAIL
}

void MainLogger::Succ(const std::string& aString)
{
	const std::string& timestamp = Timestamp();
	std::string message = "[" + timestamp + "] [ SUCCESS ] " + aString;
	myHistory << message + "\n";

#ifndef _RETAIL
	if (shouldPrintToOutput)
	{
		OutputDebugStringA(message.c_str());
	}
	else
	{
		std::unique_lock lock(myCoutMutex);
		SetConsoleTextAttribute(myHandle, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		std::cout << "[" << timestamp << "]";
		SetConsoleTextAttribute(myHandle, BACKGROUND_GREEN);
		std::cout << " [ SUCCESS ] ";
		SetConsoleTextAttribute(myHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << aString << std::endl;
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
#endif // !_RETAIL
}

void MainLogger::Succ(const std::string& aString, const std::string& aNameSpace)
{
	const std::string& timestamp = Timestamp();
	std::string message = "[" + timestamp + "] [ " + aNameSpace + " ] [ SUCCESS ] " + aString;
	myHistory << message + "\n";

#ifndef _RETAIL
	if (shouldPrintToOutput)
	{
		OutputDebugStringA(message.c_str());
	}
	else
	{
		std::unique_lock lock(myCoutMutex);
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "[" << timestamp << "] [ " << aNameSpace << " ]";
		SetConsoleTextAttribute(myHandle, BACKGROUND_GREEN);
		std::cout << " [ SUCCESS ] ";
		SetConsoleTextAttribute(myHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << aString << std::endl;
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
#endif // !_RETAIL
}

void MainLogger::LogException(const std::exception& anException)
{
	LogException(anException, 0);
}

void MainLogger::LogException(const std::exception& anException, const std::string& aNameSpace)
{
	LogException(anException, aNameSpace, 0);
}

void MainLogger::NewLine()
{
	myHistory << "\n";
#ifndef _RETAIL
	std::cout << std::endl;
#endif // !_RETAIL
}

void MainLogger::LogException(const std::exception& anException, unsigned aLevel)
{
	const std::string& timestamp = Timestamp();
	std::string message = "[" + timestamp + "] " + std::string(aLevel, ' ') + "[  FATAL  ] " + anException.what();
	myHistory << message + "\n";

#ifndef _RETAIL
	if (shouldPrintToOutput)
	{
		OutputDebugStringA(message.c_str());
	}
	else
	{
		std::unique_lock lock(myCoutMutex);
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "[" << timestamp << "]";
		std::cout << std::string(aLevel, ' ').c_str();
		SetConsoleTextAttribute(myHandle, BACKGROUND_RED);
		std::cout << " [  FATAL  ] ";
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << anException.what() << std::endl;
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
#endif // !_RETAIL

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

void MainLogger::LogException(const std::exception& anException, const std::string& aNameSpace, unsigned aLevel)
{
	const std::string& timestamp = Timestamp();
	std::string message = "[" + timestamp + "] [ " + aNameSpace + " ]" + std::string(aLevel, ' ') + "[  FATAL  ] " + anException.what();
	myHistory << message + "\n";

#ifndef _RETAIL
	if (shouldPrintToOutput)
	{
		OutputDebugStringA(message.c_str());
	}
	else
	{
		std::unique_lock lock(myCoutMutex);
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "[" << timestamp << "] [ " << aNameSpace << " ]";
		std::cout << std::string(aLevel, ' ').c_str();
		SetConsoleTextAttribute(myHandle, BACKGROUND_RED);
		std::cout << " [  FATAL  ] ";
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << anException.what() << std::endl;
		SetConsoleTextAttribute(myHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
#endif // !_RETAIL

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
