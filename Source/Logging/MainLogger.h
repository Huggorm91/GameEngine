#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <mutex>
#include <sstream>

// Threadsafe printing to cout
class MainLogger
{
public:
	MainLogger();
	~MainLogger() = default;

	void PrintHistoryToFile(const std::string& aFileName);

	// Tells the logger to output to the Visual Studio output window instead of a console.
	void SetPrintToVSOutput(bool aNewValue);

	// Log a message.
	void Log(const std::string& aString);
	void Log(const std::string& aString, const std::string& aNameSpace);

	// Log a warning message.
	void Warn(const std::string& aString);
	void Warn(const std::string& aString, const std::string& aNameSpace);

	// Log an error.
	void Err(const std::string& aString);
	void Err(const std::string& aString, const std::string& aNameSpace);

	// Log a success message.
	void Succ(const std::string& aString);
	void Succ(const std::string& aString, const std::string& aNameSpace);

	// Log an exception. Will step through nested exceptions if there are any.
	void LogException(const std::exception& anException);
	void LogException(const std::exception& anException, const std::string& aNameSpace);

	// Just force the log to go to next line.
	void NewLine();

private:
	bool shouldPrintToOutput;
	HANDLE myHandle;
	std::mutex myCoutMutex;
	std::stringstream myHistory;

	void LogException(const std::exception& anException, unsigned aLevel);
	void LogException(const std::exception& anException, const std::string& aNameSpace, unsigned aLevel);
};
