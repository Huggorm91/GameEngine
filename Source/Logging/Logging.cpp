#include "Logging.h"
#include "MainLogger.h"

MainLogger* Logger::ourMainLogger = nullptr;

Logger::Logger(const std::string& aNamespace)
{
	myNamespace = aNamespace;
}

Logger Logger::Create(const std::string& aNamespace)
{
	Logger aLogger(aNamespace);
	aLogger.Succ("Logging started for " + aNamespace);
	return aLogger;
}

void Logger::Log(const std::string& aString) const
{
	ourMainLogger->Log(aString, myNamespace);
}

void Logger::Warn(const std::string& aString) const
{
	ourMainLogger->Warn(aString, myNamespace);
}

void Logger::Err(const std::string& aString) const
{
	ourMainLogger->Err(aString, myNamespace);
}

void Logger::Succ(const std::string& aString) const
{
	ourMainLogger->Succ(aString, myNamespace);
}

void Logger::LogException(const std::exception& anException) const
{
	ourMainLogger->LogException(anException, myNamespace);
}

void Logger::NewLine() const
{
	ourMainLogger->NewLine();
}
