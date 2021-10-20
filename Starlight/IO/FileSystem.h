#pragma once
#include <string>
#include <vector>

// C++ 17 Standards. We can implement for lower standard versions with <sys>.
// General rule of thumb: Any directory references will include a / as the final character.

namespace IO
{
    class FileSystem
    {
    public:
        // File Creation
        static void CreateTextFile(const std::string& filePath, const std::string& text);
         
        // Path Manipulations/Checks
        static void OpenDirectory(const std::string& filePath);
        static bool CreateDirectory_(const std::string& filePath);
        static bool IsDirectory(const std::string& filePath);
        static bool IsFile(const std::string& filePath);
        static bool Exists(const std::string& filePath);
        static bool Delete(const std::string& filePath);
        static bool CopyFileTo(const std::string& oldPath, const std::string& newPath);
        static std::string GetCurrentDirectory_();

        static std::string GetFileNameFromFilePath(const std::string& filePath);
        static std::string GetFileNameWithoutExtensionFromFilePath(const std::string& filePath);
        static std::string GetDirectoryFromFilePath(const std::string& filePath);
        static std::string GetFilePathWithoutExtension(const std::string& filePath);
        static std::string GetExtensionFromFilePath(const std::string& filePath);
        static std::string ReplaceExtension(const std::string& filePath, const std::string& extension);
        static std::vector<std::string> GetDirectoriesInDirectory(const std::string& filePath);
        static std::vector<std::string> GetFilesInDirectory(const std::string& filePath);

        // Belongs in a higher level project settings system.
        static std::string GetProjectDirectory() { return m_ProjectDirectory; }
        static void SetProjectDirectory(const std::string& filePath) { m_ProjectDirectory = filePath; }

    private:
        static std::string m_ProjectDirectory;
    };
}