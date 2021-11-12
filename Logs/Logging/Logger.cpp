#include "Logger.h"
#include <cstdarg>

namespace Starlight
{
	Logger::Logger(const std::string& loggerName) : m_LoggerName(loggerName)
	{

	}

	void Logger::WriteInitializationLog(std::string logMessage, ...)
	{
		char buffer[2048] = { 0 };

		va_list args;
		va_start(args, logMessage);
		auto w = vsnprintf(buffer, sizeof(buffer), logMessage.c_str(), args);
		va_end(args);

		WriteLogMessage(buffer, LogLevel::Initialization);
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

	void Logger::WriteCriticalLog(std::string logMessage, ...)
	{
		char buffer[2048] = { 0 };

		va_list args;
		va_start(args, logMessage);
		auto w = vsnprintf(buffer, sizeof(buffer), logMessage.c_str(), args);
		va_end(args);

		WriteLogMessage(buffer, LogLevel::Critical);
		std::terminate();
	}

	void Logger::WriteLogMessage(const std::string& logMessage, const LogLevel& logLevel)
	{
		LogPackage logPackage(logMessage, m_LoggerName, logLevel, RetrieveCurrentTime(LogTimeMode::Console));
		PatternFormatter(logPackage);

		if (m_LogBacktracing)
		{
			m_BacktraceBuffer.PushBackLogPackage(logPackage);
		}

		if (m_LogToFile)
		{
			std::string logPrefix; 

			switch (logPackage.m_LogLevel)	//Adds the prefix manually as we can't see colors in the file.
			{
				case LogLevel::Initialization:
					logPrefix = "[INITIALIZE] ";
					break;

				case LogLevel::Info:
					logPrefix = "[INFO] ";
					break;

				case LogLevel::Warning:
					logPrefix = "[WARNING] ";
					break;

				case LogLevel::Error:
					logPrefix = "[ERROR] ";
					break;

				case LogLevel::Critical:
					logPrefix = "[CRITICAL] ";
					break;

				default:
					logPrefix = "No levels found.";
			}

			logPrefix.append(logPackage.m_LogMessage);
			LogToFile(logPrefix);
		}

		LogString(logPackage);
	}

	std::string Logger::PatternFormatter(LogPackage& logPackage)
	{
		std::string logPrefix;
		logPrefix.append("[");
		logPrefix.append(logPackage.m_LogTime);     
		logPrefix.append("] ");
		logPrefix.append(logPackage.m_LoggerName);
		logPrefix.append(": ");
		logPrefix.append(logPackage.m_LogMessage);
		logPrefix.append("\n");

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

	//====================================================================================================

	void Logger::EnableFileLogging(bool value)
	{
		if (value != m_LogToFile)
		{
			m_LogToFile = value;
		}
	}

	void Logger::EnableStartupLogging(bool value)
	{
		if (value != m_StartupFileLogging)
		{
			m_StartupFileLogging = value;
		}

		if (m_LogToFile && value)
		{
			//Each time the program runs, we should create a new file with time and date as its name if enabled.
			m_LogFileName = FileSystemA::CreateNewFile(RetrieveCurrentTime(LogTimeMode::File));
		}
	}

	void Logger::EnableDailyLoggingTimer(bool value, int hour, int minutes)
	{
		if (value != m_DailyFileLogging)
		{
			m_DailyFileLogging = value;
		}

		if (m_LogToFile && value)
		{

		}
	}

	void Logger::EnableBacktracing(bool value, size_t messageCount)
	{
		if (value != m_LogBacktracing)
		{
			m_LogBacktracing = value;
		}

		if (value)
		{
			m_BacktraceBuffer.EnableBacktraceBuffer(messageCount);
		}
	}

	void Logger::DisableBacktracing()
	{
		m_LogBacktracing = false;
	}

	void Logger::DumpBacktracingBuffer()
	{
		if (!m_LogBacktracing)
		{
			WriteErrorLog("Cannot dump log messages. Backtracing is not enabled.");
			return;
		}

		for (const LogPackage& logPackage : m_BacktraceBuffer.RetrieveLogPackages())
		{
			LogString(logPackage);
		}

		m_BacktraceBuffer.ClearBuffer();
	}
}