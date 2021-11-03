#pragma once
#include <vector>
#include <string>

namespace IO
{
    class EngineIO
    {
        // Supported Files - Externals
        static bool IsSupportedImageFile(const std::string& filePath);
        static bool IsSupportedModelFile(const std::string& filePath);
        static bool IsSupportedAudioFile(const std::string& filePath);

        // Supported Files - Internals
        static bool IsEngineMaterialFile(const std::string& filePath);
        static bool IsEngineSceneFile(const std::string& filePath);
        static bool IsEngineFile(const std::string& filePath);

        // Supported Files
        static std::vector<std::string> GetSupportedSceneFilesInDirectory(const std::string& filePath);
        static std::vector<std::string> GetSupportedModelFilesInDirectory(const std::string& filePath);

    };

    static const char* g_Extension_Scene = ".starlight";
    static const char* g_Extension_Material = ".material";
    static const char* g_Extension_Prefab = ".prefab";
    static const char* g_Extension_Texture = ".texture";

    static const std::vector<std::string> g_SupportedImageFormats
    {
        ".jpg",
        ".png",
        ".ico"
    };

    static const std::vector<std::string> g_SupportedAudioFormats
    {
        ".mp3",
        ".wav",
        ".ogg"
    };

    static const std::vector<std::string> g_SupportedModelFormats
    {
        ".obj",
        ".fbx",
        ".gltf"
    };


}