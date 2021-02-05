#pragma once
#include <string>

namespace Starlight
{
	class FileSystem
	{
	public:
		std::string CreateNewFile(const std::string& filePath);
		void ChangeLogDirectory(const std::string& fileDirectory);

	private:
		std::string m_LogDirectory = "Logs/";
		std::string m_LogFilePostfix = ".txt";
	}; 
}