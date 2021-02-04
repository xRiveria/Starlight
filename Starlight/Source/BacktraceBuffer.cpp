#include "BacktraceBuffer.h"

namespace Starlight
{
	void BacktraceBuffer::EnableBacktraceBuffer(size_t messageCount)
	{
		m_MaximumBufferSize = messageCount;
	}

	void BacktraceBuffer::PushBackLogPackage(const LogPackage& logPackage)
	{
		m_LogPackages.push_back(logPackage);
	}

	void BacktraceBuffer::ClearBuffer()
	{
		m_LogPackages.clear();
	}
}