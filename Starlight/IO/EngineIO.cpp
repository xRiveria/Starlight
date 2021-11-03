#include "EngineIO.h"
#include "FileSystem.h"
#include "StringUtilities.h"

namespace IO
{
    bool EngineIO::IsSupportedImageFile(const std::string& filePath)
    {
        const std::string currentExtension = FileSystem::GetExtensionFromFilePath(filePath);

        for (const auto& format : g_SupportedImageFormats)
        {
            if (currentExtension == format || currentExtension == StringUtilities::ConvertToUppercase(format))
            {
                return true;
            }
        }

        if (currentExtension == g_Extension_Texture)
        {
            return true;
        }

        return false;
    }

    bool EngineIO::IsSupportedModelFile(const std::string& filePath)
    {
        const std::string currentExtension = FileSystem::GetExtensionFromFilePath(filePath);

        for (const auto& format : g_SupportedModelFormats)
        {
            if (currentExtension == format || currentExtension == StringUtilities::ConvertToUppercase(format))
            {
                return true;
            }
        }

        return false;
    }

    bool EngineIO::IsSupportedAudioFile(const std::string& filePath)
    {
        const std::string currentExtension = FileSystem::GetExtensionFromFilePath(filePath);

        for (const auto& format : g_SupportedAudioFormats)
        {
            if (currentExtension == format || currentExtension == StringUtilities::ConvertToUppercase(format))
            {
                return true;
            }
        }

        return false;
    }

    bool EngineIO::IsEngineMaterialFile(const std::string& filePath)
    {
        return FileSystem::GetExtensionFromFilePath(filePath) == g_Extension_Material;
    }

    bool EngineIO::IsEngineSceneFile(const std::string& filePath)
    {
        return FileSystem::GetExtensionFromFilePath(filePath) == g_Extension_Scene;
    }

    bool EngineIO::IsEngineFile(const std::string& filePath)
    {
        return IsEngineMaterialFile(filePath) || IsEngineSceneFile(filePath);
    }

    std::vector<std::string> EngineIO::GetSupportedSceneFilesInDirectory(const std::string& filePath)
    {
        return std::vector<std::string>();
    }

    std::vector<std::string> EngineIO::GetSupportedModelFilesInDirectory(const std::string& filePath)
    {
        return std::vector<std::string>();
    }
}