#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "FileSystemA.h"

namespace Starlight
{
	std::string FileSystemA::m_LogDirectory = "../Logs/";
	std::string FileSystemA::m_LogFilePostfix = ".txt";

	std::string FileSystemA::CreateNewFile(const std::string& filePath)
	{
		std::ofstream createdFile(m_LogDirectory + filePath + m_LogFilePostfix);

		return m_LogDirectory + filePath + m_LogFilePostfix;
	}

	void FileSystemA::ChangeLogDirectory(const std::string& fileDirectory)
	{
		m_LogDirectory = fileDirectory;
	}
}

