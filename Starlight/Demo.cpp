#include "Source/Logger.h"
#include <memory>

int main(int argc, int argv[])
{
	std::shared_ptr<Starlight::Logger> m_Logger = std::make_shared<Starlight::Logger>("Engine Logger");

	m_Logger->EnableFileLogging(true);
	m_Logger->EnableBacktracing(5);

	m_Logger->WriteInfoLog("Initialized %s", "Starlight Logger!");
	m_Logger->WriteWarningLog("%d GPU Cycles Recorded!", 5);
	m_Logger->WriteErrorLog("Your backend named %s has crashed!", "Aurora");

	m_Logger->DumpBacktracingBuffer();
}