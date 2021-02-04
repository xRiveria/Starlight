#include "ConsoleState.h"

namespace Starlight
{
	ConsoleState::ConsoleState() : m_OutHandle(GetStdHandle(STD_OUTPUT_HANDLE))
	{
		InitializeLogLevelColors();
		GetConsoleScreenBufferInfo(m_OutHandle, &m_DefaultConsoleState);
	}

	ConsoleState::~ConsoleState()
	{
	}

	void ConsoleState::SetConsoleTextColor(const LogLevel& logLevel)
	{
		SetConsoleTextAttribute(m_OutHandle, m_LogLevelColorMap[logLevel]);
	}

	void ConsoleState::RestoreConsoleAttributes()
	{
		SetConsoleTextAttribute(m_OutHandle, m_DefaultConsoleState.wAttributes);
	}

    void ConsoleState::InitializeLogLevelColors()
	{
		m_LogLevelColorMap[LogLevel::Initialization] = GreenConsoleTextColor;
		m_LogLevelColorMap[LogLevel::Info] = GreenConsoleTextColor;
		m_LogLevelColorMap[LogLevel::Warning] = YellowConsoleTextColor;
		m_LogLevelColorMap[LogLevel::Error] = RedConsoleTextColor;
	}
}