#pragma once
#include <string>

namespace Starlight
{
	class FileSystem
	{
	public:
		static std::string CreateNewFile(const std::string& filePath);
		static void ChangeLogDirectory(const std::string& fileDirectory);

	private:
		static std::string m_LogDirectory;
		static std::string m_LogFilePostfix;
	};
}