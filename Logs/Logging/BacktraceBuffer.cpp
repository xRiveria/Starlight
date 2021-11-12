#include "BacktraceBuffer.h"

namespace Starlight
{
	void BacktraceBuffer::EnableBacktraceBuffer(size_t messageCount)
	{
		m_MaximumBufferSize = messageCount;
	}

	void BacktraceBuffer::PushBackLogPackage(const LogPackage& logPackage)
	{
		//Prevent overflowing.
		if (m_LogPackages.size() + 1 > m_MaximumBufferSize)
		{
			m_LogPackages.erase(m_LogPackages.begin());
		}

		m_LogPackages.push_back(logPackage);
	}

	void BacktraceBuffer::ClearBuffer()
	{
		m_LogPackages.clear();
	}
}