#include "Demo.h"
#include <memory>

namespace Application
{
	std::shared_ptr<Starlight::Logger> Log::s_ApplicationLogger = nullptr;

	void Log::InitializeLogger()
	{
		s_ApplicationLogger = std::make_shared<Starlight::Logger>("Aurora Engine"); //Creates a global logger object.
	}
}

int main(int argc, int argv[])
{
	//Example way to setup the system for use with your engine. 
	Application::Log::InitializeLogger();

	//File Logging Support
	{
		//Enables support to store your logs into a text file. This is a master switch that does nothing on its own. We must also enable its associative features. 
		Application::Log::GetInstance()->EnableFileLogging(true); 
		//Creates a new log file everytime the application launches.
		Application::Log::GetInstance()->EnableStartupLogging(true);
		//Starts a daily timer that creates a log file at 5:30am everyday.
		Application::Log::GetInstance()->EnableDailyLoggingTimer(true, 5, 30);
		//Changes the log directory for your text logs.
		// Starlight::FileSystem::ChangeLogDirectory("LogsFolder/");
	}

	//Backtracing Support
	{
		//Enables support to store your logs into a buffer to be retrieved at will. Second argument states the amount of messages to be stored up to at any given point.
		Application::Log::GetInstance()->EnableBacktracing(true, 5);
	}

	//Different ways to log your system.
	{
		ENGINE_LOG_INITIALIZATION("Initialized %s", "Starlight Logger!");
		ENGINE_LOG_INFO("%d enemies destroyed in this level.", 51);
		ENGINE_LOG_WARN("%d GPU Cycles Recorded! Bad!", 5);
		ENGINE_LOG_ERROR("Your backend named %s has crashed!", "Aurora");
		//ENGINE_LOG_CRITICAL("Core system error!"); //Critical errors will terminate your program.
	}

	//Outputs the messages you've stored in a backbuffer if enabled.
	std::cout << "Dumping backtrace buffer..." << "\n";
	Application::Log::GetInstance()->DumpBacktracingBuffer();

	std::cerr << "Error!\n"; // Unbuffered standard stream. Displays the error message immediately.
}