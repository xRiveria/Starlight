#pragma once
#include <string>

enum class LogLevel
{
	Info,
	Warning,
	Error
};

struct LogPackage
{
	LogPackage(const std::string& logMessage, const LogLevel& logLevel) : m_LogMessage(logMessage), m_LogLevel(logLevel)
	{

	}
	
	std::string m_LogMessage = "Nothing.";
	LogLevel m_LogLevel;
};
