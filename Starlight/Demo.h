#pragma once
#include "Source/Logger.h"
#include <memory>

namespace Application
{
	class Log
	{
	public:
		static void InitializeLogger();
		static std::shared_ptr<Starlight::Logger>& GetInstance() { return s_ApplicationLogger; }

	private:
		static std::shared_ptr<Starlight::Logger> s_ApplicationLogger;
	};
};

//Core Logging Macros
#define ENGINE_LOG_INITIALIZATION(...)	::Application::Log::GetInstance()->WriteInitializationLog(__VA_ARGS__);
#define ENGINE_LOG_INFO(...)			::Application::Log::GetInstance()->WriteInfoLog(__VA_ARGS__);
#define ENGINE_LOG_WARN(...)			::Application::Log::GetInstance()->WriteWarningLog(__VA_ARGS__);
#define ENGINE_LOG_ERROR(...)			::Application::Log::GetInstance()->WriteErrorLog(__VA_ARGS__);
#define ENGINE_LOG_CRITICAL(...)		::Application::Log::GetInstance()->WriteCriticalLog(__VA_ARGS__);