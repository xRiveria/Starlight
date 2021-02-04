#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <string.h>
#include <Windows.h>

enum LogLevel
{
	Initialization,
	Info,
	Warning,
	Error,
	Critical,
	TotalLevels
};

enum LogTimeMode
{
	File,
	Console
};

struct LogPackage
{
	LogPackage(const std::string& logMessage, const std::string& loggerName, const LogLevel& logLevel, const std::string& logTime) 
		: m_LogMessage(logMessage), m_LoggerName(loggerName), m_LogLevel(logLevel), m_LogTime(logTime)
	{

	}
	
	std::string m_LogMessage = "Message";
	std::string m_LoggerName = "Logger";
	std::string m_LogTime = "Time";
	LogLevel m_LogLevel = LogLevel::TotalLevels;
};

inline std::string RetrieveCurrentTime(const LogTimeMode& timeMode)
{
	std::chrono::time_point currentTimePoint = std::chrono::system_clock::now();
	time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);
	std::tm buffer;
	localtime_s(&buffer, &currentTime);

	std::stringstream transTime;

	switch (timeMode)
	{
		case LogTimeMode::File:
			transTime << std::put_time(&buffer, "%B_%d_%Y_%HH_%MM_%SS");
			break;

		case LogTimeMode::Console:
			transTime << std::put_time(&buffer, "%HH:%MM:%SS");
			break;
	}

	return transTime.str();
}

const WORD BoldConsoleTextColor = FOREGROUND_INTENSITY;
const WORD BlueConsoleTextColor = 11; 
const WORD GreenConsoleTextColor = FOREGROUND_GREEN;
const WORD YellowConsoleTextColor = FOREGROUND_RED | FOREGROUND_GREEN;
const WORD RedConsoleTextColor = FOREGROUND_RED;
const WORD CriticalConsoleTextColor = 12;
