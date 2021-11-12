#pragma once
#include "LogUtilities.h"
#include "ConsoleState.h"
#include "BacktraceBuffer.h"
#include "FileSystemA.h"
#include <string>
#include <vector>
#include <fstream>

namespace Starlight
{
	/*
		Logger object that exposes state functionality toggles and logging capabilities. 
	*/

	class Logger
	{
	public:
		Logger(const std::string& loggerName);
		~Logger() = default;
		 
		// Logging Levels
		void WriteInitializationLog(std::string logMessage, ...);
		void WriteInfoLog(std::string logMessage, ...);
		void WriteWarningLog(std::string logMessage, ...);
		void WriteErrorLog(std::string logMessage, ...);
		void WriteCriticalLog(std::string logMessage, ...);

		// Backtracing Support. Efficiently stores all debug messages in a buffer/queue until needed for debugging.
		void EnableBacktracing(bool value, size_t messageCount);
		void DisableBacktracing();
		void DumpBacktracingBuffer();

		// File Logging Support
		void EnableFileLogging(bool value);
		void EnableStartupLogging(bool value);
		void EnableDailyLoggingTimer(bool value, int hour, int minutes);

	private:
		std::string PatternFormatter(LogPackage& logPackage);
		void WriteLogMessage(const std::string& logMessage , const LogLevel& logLevel);

		void LogString(const LogPackage& logPackage); //Everything resolves to this.
		void LogToFile(const std::string& logMessage);

	private:
		std::string m_LogFileName = "Logs/Log.txt";
		size_t m_LogFileIndex = 0;

		std::string m_LoggerName;
		
		//States
		bool m_LogToFile = false;
		bool m_StartupFileLogging = false;
		bool m_DailyFileLogging = false;

		BacktraceBuffer m_BacktraceBuffer;
		bool m_LogBacktracing = false;
		
		ConsoleState m_ConsoleState;

		//File Operations
		std::ofstream m_FileOut;
	};
}