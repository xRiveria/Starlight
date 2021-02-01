#pragma once
#include "LoggerCommons.h"
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
		void WriteInfoLog(std::string logMessage, ...);
		void WriteWarningLog(std::string logMessage, ...);
		void WriteErrorLog(std::string logMessage, ...);

		//Backtracing Support. Efficiently stores all debug messages in a buffer/queue until needed for debugging.
		void EnableBacktracing(size_t messageCount);
		void DisableBacktracing();
		void DumpBacktracingBuffer();

		//File Logging Support
		void EnableFileLogging(const bool& value);


	private:
		std::string PatternFormatter(const LogPackage& logPackage);
		void WriteLogMessage(const std::string& logMessage , const LogLevel& logLevel);

		void LogString(const std::string& logMessage); //Everything resolves to this.
		void LogToFile(const std::string& logMessage);

	private:
		std::string m_LogFileName = "Logs/Log.txt";
		std::string m_LoggerName;
		
		//States
		bool m_LogToFile = false;
		bool m_LogBacktracing = false;
		std::vector<std::string> m_LogBuffer;

		//File Operations
		std::ofstream m_FileOut;
	};
}