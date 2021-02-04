#pragma once
#include "LogCommons.h"
#include <vector>

namespace Starlight
{
	class BacktraceBuffer
	{
	public:
		void EnableBacktraceBuffer(size_t messageCount);
		void PushBackLogPackage(const LogPackage& logPackage);
		void ClearBuffer();

		std::vector<LogPackage>& RetrieveLogPackages() { return m_LogPackages; }

	private:
		size_t m_MaximumBufferSize;
		std::vector<LogPackage> m_LogPackages;
	};
}
