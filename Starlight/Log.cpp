#include "Log.h"

namespace Application
{
	std::shared_ptr<Starlight::Logger> Log::s_ApplicationLogger = nullptr;

	void Log::InitializeLogger()
	{
		s_ApplicationLogger = std::make_shared<Starlight::Logger>("Application Name");

		s_ApplicationLogger->EnableFileLogging(true);
		s_ApplicationLogger->EnableBacktracing(5);
	}
}