#pragma once
#include <Windows.h>
#include <string>
#include <vector>

namespace SystemInfo
{
    struct CPUInfo
    {
        std::string m_ManufacturerName;
        std::string m_DeviceName;
        unsigned int m_NumberOfCores;
        unsigned int m_NumberOfThreads;
        unsigned int m_ModelID;
    };

    struct GPUInfo
    {
        std::string m_DeviceName;
        unsigned int m_DeviceID;
        unsigned int m_VendorID;
        size_t m_DedicatedGPUMemory;
    };

    struct RAMInfo
    {
        unsigned int m_UsagePercentage;
        unsigned long long m_TotalPhysicalMemory;
        unsigned long long m_FreePhysicalMemory;
        unsigned long long m_TotalPageFileSize;
        unsigned long long m_FreePageFileSize;
        unsigned long long m_TotalVirtualMemory;
        unsigned long long m_FreeVirtualMemory;
        unsigned long long m_ExtendedMemory;
    };

    struct MonitorInfo
    {
        struct Resolution
        {
            int m_Width;
            int m_Height;
        };

        struct Mode
        {
            Resolution m_Resolution;
            unsigned int m_RefreshRate;
        };

        std::string m_DeviceName;
        std::string m_LogicalDeviceName;
        std::string m_DeviceID;
        Resolution m_NativeResolution;    
        bool m_IsHDRSupported;
        unsigned int m_RotationDegrees;
        std::vector<Mode> m_SupportedModes;
        Mode m_HighestMode;

        static bool CheckHDRSupport(HWND hwnd);
    };

    struct SystemInfo
    {
        CPUInfo m_CPU;
        std::vector<GPUInfo> m_GPUInfos;
        std::vector<MonitorInfo> m_MonitorInfos;
        RAMInfo m_RAMInfo;
    };

    CPUInfo GetCPUInfo();
    std::vector<GPUInfo> GetGPUInfo();
    std::vector<MonitorInfo> GetDisplayInfo();
    RAMInfo GetRAMInfo();
    SystemInfo GetSystemInfo();

    std::string PrintSystemInfo(const SystemInfo& systemInfo);
}