#include "FileSystem.h"
#include <fstream>
#include <filesystem>
#include <iostream>

namespace IO
{
    std::string FileSystem::m_ProjectDirectory = "";

	void FileSystem::CreateTextFile(const std::string& filePath, const std::string& text)
	{
		std::ofstream outputFile(filePath);
		outputFile << text;
		outputFile.flush();
		outputFile.close();
	}

	void FileSystem::OpenDirectory(const std::string& filePath)
	{

	}

	bool FileSystem::CreateDirectory_(const std::string& filePath)
	{
		try
		{
			if (std::filesystem::create_directories(filePath))
			{
				return true;
			}
		}
		catch (const std::filesystem::filesystem_error& error)
		{
			std::cout << filePath << " - " << error.what() << "\n";
		}

		return false;
	}

	bool FileSystem::IsDirectory(const std::string& filePath)
	{
		try
		{
			if (Exists(filePath) && std::filesystem::is_directory(filePath))
			{
				return true;
			}
		}
		catch (std::filesystem::filesystem_error& error)
		{
			std::cout << filePath << " - " << error.what() << "\n";
		}

		return false;
	}

	bool FileSystem::Delete(const std::string& filePath)
	{
		try
		{
			if (Exists(filePath) && std::filesystem::remove_all(filePath))
			{
				return true;
			}
		}
		catch (const std::filesystem::filesystem_error& error)
		{
			std::cout << filePath << " - " << error.what() << "\n";
		}

		return false; 
	}

	bool FileSystem::CopyFileTo(const std::string& oldPath, const std::string& newPath)
	{
		return false;
	}

	std::string FileSystem::GetFileNameFromFilePath(const std::string& filePath)
	{
		if (FileSystem::Exists(filePath))
		{
			return std::filesystem::path(filePath).filename().generic_string();
		}
		else
		{
			std::cout << "Attempted to retrieve file name of a file that does not exist: " << filePath << ".\n";
		}

		return "File Error";
	}

	std::string FileSystem::GetFileNameWithoutExtensionFromFilePath(const std::string& filePath)
	{
		const std::string fileName = GetFileNameFromFilePath(filePath);
		const size_t lastExtensionIndex = fileName.find_last_of('.');

		if (lastExtensionIndex != std::string::npos)
		{
			return fileName.substr(0, lastExtensionIndex);
		}

		return fileName;
	}

	std::string FileSystem::GetRootDirectory(const std::string& filePath)
	{
		return std::filesystem::path(filePath).root_directory().generic_string();
	}

	std::string FileSystem::GetParentDirectory(const std::string& filePath)
	{
		return std::filesystem::path(filePath).parent_path().generic_string();
	}

	std::string FileSystem::GetFilePathWithoutExtension(const std::string& filePath)
	{
		return GetDirectoryFromFilePath(filePath) + GetFileNameWithoutExtensionFromFilePath(filePath);
	}

	std::string FileSystem::GetDirectoryFromFilePath(const std::string& filePath)
	{
		const size_t lastDirectoryIndex = filePath.find_last_of("\\/");

		if (lastDirectoryIndex != std::string::npos)
		{
			return filePath.substr(0, lastDirectoryIndex + 1); // To include the / itself.
		}

		return filePath;
	}

	bool FileSystem::Exists(const std::string& filePath)
	{
		try
		{
			if (std::filesystem::exists(filePath))
			{
				return true;
			}
		}
		catch (const std::filesystem::filesystem_error& error)
		{
			std::cout << filePath << " - " << error.what() << "\n";
		}

		return false;
	}

	bool FileSystem::IsFile(const std::string& filePath)
	{
		try
		{
			if (std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath))
			{
				return true;
			}
		}
		catch (const std::filesystem::filesystem_error& error)
		{
			std::cout << filePath << " - " << error.what() << "\n";
		}

		return false;
	}

	
	std::string FileSystem::GetExtensionFromFilePath(const std::string& filePath)
	{
		const size_t fileExtension = filePath.find_last_of('.');

		if (fileExtension != std::string::npos)
		{
			return filePath.substr(fileExtension, filePath.size());
		}

		return "Extension Error";
	}

	std::string FileSystem::GetCurrentDirectory_()
	{
		return std::filesystem::current_path().string();
	}

	std::string FileSystem::ReplaceExtension(const std::string& filePath, const std::string& extension)
	{
		return GetDirectoryFromFilePath(filePath) + GetFileNameWithoutExtensionFromFilePath(filePath) + extension;
	}

	std::vector<std::string> FileSystem::GetDirectoriesInDirectory(const std::string& filePath)
	{
		const std::string path = filePath.empty() ? GetCurrentDirectory_() : filePath;

		std::vector<std::string> directories;

		const std::filesystem::directory_iterator iteratorEnd;

		try
		{
			for (std::filesystem::directory_iterator iterator(path); iterator != iteratorEnd; ++iterator)
			{
				if (!std::filesystem::is_directory(iterator->status()))
				{
					continue;
				}

				directories.emplace_back(iterator->path().string());
			}
		}
		catch (const std::system_error& systemError)
		{
			std::cout << filePath << " - " << systemError.what() << "\n";
		}

		return directories;
	}

	std::vector<std::string> FileSystem::GetFilesInDirectory(const std::string& filePath)
	{
		const std::string path = filePath.empty() ? GetCurrentDirectory_() : filePath;

		std::vector<std::string> directoryFiles;

		const std::filesystem::directory_iterator iteratorEnd; // Default constructor yields past the last entry.

		try
		{
			for (std::filesystem::directory_iterator iterator(path); iterator != iteratorEnd; ++iterator)
			{
				if (!std::filesystem::is_regular_file(iterator->status()))
				{
					continue;
				}

				directoryFiles.emplace_back(iterator->path().string());
			}
		}
		catch (const std::system_error& systemError)
		{
			std::cout << filePath << " - " << systemError.what() << "\n";
		}

		return directoryFiles;
	}

}