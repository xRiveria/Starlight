#pragma once
#include <string>
#include <Windows.h>

enum LogLevel
{
	Initialization,
	Info,
	Warning,
	Error,
	TotalLevels
};

struct LogPackage
{
	LogPackage(const std::string& logMessage, const std::string& loggerName, const LogLevel& logLevel) : m_LogMessage(logMessage), m_LoggerName(loggerName), m_LogLevel(logLevel)
	{

	}
	
	std::string m_LogMessage = "Message";
	std::string m_LoggerName = "Logger";
	LogLevel m_LogLevel = LogLevel::TotalLevels;
};

const WORD GreenConsoleTextColor = FOREGROUND_GREEN;
const WORD YellowConsoleTextColor = FOREGROUND_RED | FOREGROUND_GREEN;
const WORD RedConsoleTextColor = FOREGROUND_RED;
