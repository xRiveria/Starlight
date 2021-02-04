#include "Logger.h"
#include <cstdarg>
#include <iostream>

namespace Starlight
{
	Logger::Logger(const std::string& loggerName) : m_LoggerName(loggerName)
	{
	}

	void Logger::WriteInfoLog(std::string logMessage, ...)
	{
		char buffer[2048] = { 0 };

		va_list args;
		va_start(args, logMessage);
		auto w = vsnprintf(buffer, sizeof(buffer), logMessage.c_str(), args);
		va_end(args);

		WriteLogMessage(buffer, LogLevel::Info);
	}

	void Logger::WriteWarningLog(std::string logMessage, ...)
	{
		char buffer[2048] = { 0 };

		va_list args;
		va_start(args, logMessage);
		auto w = vsnprintf(buffer, sizeof(buffer), logMessage.c_str(), args);
		va_end(args);

		WriteLogMessage(buffer, LogLevel::Warning);
	}

	void Logger::WriteErrorLog(std::string logMessage, ...)
	{
		char buffer[2048] = { 0 };

		va_list args;
		va_start(args, logMessage);
		auto w = vsnprintf(buffer, sizeof(buffer), logMessage.c_str(), args);
		va_end(args);

		WriteLogMessage(buffer, LogLevel::Error);
	}

	void Logger::WriteLogMessage(const std::string& logMessage, const LogLevel& logLevel)
	{
		LogPackage logPackage(logMessage, m_LoggerName, logLevel);
		PatternFormatter(logPackage);

		if (m_LogBacktracing)
		{
			m_LogBuffer.emplace_back(logPackage);
		}

		if (m_LogToFile)
		{
			LogToFile(logPackage.m_LogMessage);
		}

		LogString(logPackage);
	}

	std::string Logger::PatternFormatter(LogPackage& logPackage)
	{
		std::string logPrefix;
		std::string loggerNamePostfix = ": ";

		switch (logPackage.m_LogLevel)
		{
			case LogLevel::Info:
				logPrefix = "[INFO] ";
				break;

			case LogLevel::Warning:
				logPrefix = "[WARNING] ";
				break;

			case LogLevel::Error:
				logPrefix = "[ERROR] ";
				break;

			default:
				logPrefix = "No levels found.";
		}

		logPrefix = logPrefix + logPackage.m_LoggerName + loggerNamePostfix + logPackage.m_LogMessage + std::string("\n");
		logPackage.m_LogMessage = logPrefix;

		return logPrefix;
	}

	void Logger::LogString(const LogPackage& logPackage) //Everything flows into here.
	{
		m_ConsoleState.SetConsoleTextColor(logPackage.m_LogLevel);

		std::cout << logPackage.m_LogMessage;

		m_ConsoleState.RestoreConsoleAttributes();
	}

	void Logger::LogToFile(const std::string& logMessage)
	{
		//Open a log file to write the messages to.
		m_FileOut.open(m_LogFileName, std::ofstream::out | std::ofstream::app);

		if (m_FileOut.is_open())
		{
			//Write out the error message.
			m_FileOut << logMessage;

			//Close the file.
			m_FileOut.close();
		}
	}

	void Logger::EnableFileLogging(const bool& value)
	{
		if (value != m_LogToFile)
		{
			m_LogToFile = value;
		}
	}

	void Logger::EnableBacktracing(size_t messageCount)
	{
		m_LogBacktracing = true;
	}

	void Logger::DisableBacktracing()
	{
		m_LogBacktracing = false;
	}

	void Logger::DumpBacktracingBuffer()
	{
		for (const LogPackage& logPackage : m_LogBuffer)
		{
			LogString(logPackage);
		}
	}
}