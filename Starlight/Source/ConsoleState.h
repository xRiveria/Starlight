#pragma once
#include "LogCommons.h"
#include <array>

namespace Starlight
{
	class ConsoleState
	{
	public:
		ConsoleState();
		~ConsoleState();

		void SetConsoleTextColor(const LogLevel& logLevel);
		void RestoreConsoleAttributes();

	private:
		void InitializeLogLevelColors();

	private:
		HANDLE m_OutHandle;
		CONSOLE_SCREEN_BUFFER_INFO m_DefaultConsoleState;
		std::array<WORD, LogLevel::TotalLevels> m_LogLevelColorMap;
	};
}