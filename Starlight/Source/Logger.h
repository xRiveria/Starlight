#pragma once
#include "LogCommons.h"
#include "ConsoleState.h"
#include "BacktraceBuffer.h"
#include "FileSystem.h"
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
		 
		//Logging Levels
		void WriteInitializationLog(std::string logMessage, ...);
		void WriteInfoLog(std::string logMessage, ...);
		void WriteWarningLog(std::string logMessage, ...);
		void WriteErrorLog(std::string logMessage, ...);
		void WriteCriticalLog(std::string logMessage, ...);

		//Backtracing Support. Efficiently stores all debug messages in a buffer/queue until needed for debugging.
		void EnableBacktracing(const bool& value, size_t messageCount);
		void DisableBacktracing();
		void DumpBacktracingBuffer();

		//File Logging Support
		void EnableFileLogging(const bool& value);
		void EnableStartupLogging(const bool& value);
		void EnableDailyLoggingTimer(const bool& value, const int& hour, const int& minutes);

		FileSystem RetrieveFileSystemHelper() { return m_FileSystemHelper; }

	private:
		std::string PatternFormatter(LogPackage& logPackage);
		void WriteLogMessage(const std::string& logMessage , const LogLevel& logLevel);

		void LogString(const LogPackage& logPackage); //Everything resolves to this.
		void LogToFile(const std::string& logMessage);

	private:
		std::string m_LogFileName = "Logs/Log.txt";
		size_t m_LogFileIndex = 0;

		std::string m_LoggerName;
		FileSystem m_FileSystemHelper;
		
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