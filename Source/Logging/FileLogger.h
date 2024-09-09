#pragma once
#include <fstream>

class FileLogger
{
public:
	FileLogger(const std::string& aFileName);
	~FileLogger();

	// Log a message.
	void Log(const std::string& aString);

	// Log a warning message.
	void Warn(const std::string& aString);

	// Log an error.
	void Err(const std::string& aString);

	// Log a success message.
	void Succ(const std::string& aString);

	// Log an exception. Will step through nested exceptions if there are any.
	void LogException(const std::exception& anException);

	// Just force the log to go to next line.
	void NewLine();

private:
	std::ofstream myLogFile;

	void LogException(const std::exception& anException, unsigned aLevel = 0);
};