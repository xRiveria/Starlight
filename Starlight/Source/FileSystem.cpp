#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "FileSystem.h"

namespace Starlight
{
	std::string FileSystem::m_LogDirectory = "Logs/";
	std::string FileSystem::m_LogFilePostfix = ".txt";

	std::string FileSystem::CreateNewFile(const std::string& filePath)
	{
		std::ofstream createdFile(m_LogDirectory + filePath + m_LogFilePostfix);

		return m_LogDirectory + filePath + m_LogFilePostfix;
	}

	void FileSystem::ChangeLogDirectory(const std::string& fileDirectory)
	{
		m_LogDirectory = fileDirectory;
	}
}