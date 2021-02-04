#include "Demo.h"
#include <memory>

namespace Application
{
	std::shared_ptr<Starlight::Logger> Log::s_ApplicationLogger = nullptr;

	void Log::InitializeLogger()
	{
		s_ApplicationLogger = std::make_shared<Starlight::Logger>("Application Name");

		s_ApplicationLogger->EnableFileLogging(true);
		s_ApplicationLogger->EnableBacktracing(1);
	}
}

int main(int argc, int argv[])
{
	//Example way to setup the system for use with your engine. 
	Application::Log::InitializeLogger();

	//Different ways to log your system.
	ENGINE_LOG_INITIALIZATION("Initialized %s", "Starlight Logger!");
	ENGINE_LOG_INFO("%d enemies destroyed in this level.", 51);
	ENGINE_LOG_WARN("%d GPU Cycles Recorded! Bad!", 5);
	ENGINE_LOG_ERROR("Your backend named %s has crashed!", "Aurora");
	//ENGINE_LOG_CRITICAL("Core system error!"); //Critical errors will terminate your program.

	//Outputs the messages you've stored in a backbuffer if enabled.
	//Application::Log::RetrieveApplicationLogger()->DumpBacktracingBuffer();

	//Check file logging.
}