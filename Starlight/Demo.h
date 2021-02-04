#pragma once
#include "Source/Logger.h"
#include <memory>

namespace Application
{
	class Log
	{
	public:
		static void InitializeLogger();
		static std::shared_ptr<Starlight::Logger>& RetrieveApplicationLogger() { return s_ApplicationLogger; }

	private:
		static std::shared_ptr<Starlight::Logger> s_ApplicationLogger;
	};
};

//Core Logging Macros
#define ENGINE_LOG_INITIALIZATION(...)	::Application::Log::RetrieveApplicationLogger()->WriteInitializationLog(__VA_ARGS__);
#define ENGINE_LOG_INFO(...)			::Application::Log::RetrieveApplicationLogger()->WriteInfoLog(__VA_ARGS__);
#define ENGINE_LOG_WARN(...)			::Application::Log::RetrieveApplicationLogger()->WriteWarningLog(__VA_ARGS__);
#define ENGINE_LOG_ERROR(...)			::Application::Log::RetrieveApplicationLogger()->WriteErrorLog(__VA_ARGS__);
#define ENGINE_LOG_CRITICAL(...)		::Application::Log::RetrieveApplicationLogger()->WriteCriticalLog(__VA_ARGS__);