#include "Log.h"
#include <memory>

int main(int argc, int argv[])
{
	Application::Log::InitializeLogger();

	ENGINE_LOG_INFO("Initialized %s", "Starlight Logger!");
	ENGINE_LOG_WARN("%d GPU Cycles Recorded!", 5);
	ENGINE_LOG_ERROR("Your backend named %s has crashed!", "Aurora");

	Application::Log::RetrieveApplicationLogger()->DumpBacktracingBuffer();
}