#pragma once
#include <string>

class Logger
{
public:
	Logger() = default;

	static Logger Create(const std::string& aNamespace);

	// Log a message.
	void Log(const std::string& aString) const;

	// Log a warning message.
	void Warn(const std::string& aString) const;

	// Log an error.
	void Err(const std::string& aString) const;

	// Log a success message.
	void Succ(const std::string& aString) const;

	// Log an exception. Will step through nested exceptions if there are any.
	void LogException(const std::exception& anException) const;

	// Just force the log to go to next line.
	void NewLine() const;

private:
	friend class Engine;
	static class MainLogger* ourMainLogger;
	std::string myNamespace;

	Logger(const std::string& aNamespace);
};