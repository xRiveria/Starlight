#include "SystemInfo.h"

namespace SystemInfo
{
    RAMInfo GetRAMInfo()
    {
        MEMORYSTATUSEX memoryStatus;
        memoryStatus.dwLength = sizeof(memoryStatus);
        GlobalMemoryStatusEx(&memoryStatus);

        RAMInfo ramInfo;
        ramInfo.m_UsagePercentage = memoryStatus.dwMemoryLoad;
        ramInfo.m_TotalPhysicalMemory = memoryStatus.ullTotalPhys;
        ramInfo.m_FreePhysicalMemory = memoryStatus.ullAvailPhys;
        ramInfo.m_TotalPageFileSize = memoryStatus.ullTotalPageFile;
        ramInfo.m_FreePageFileSize = memoryStatus.ullAvailPageFile;
        ramInfo.m_TotalVirtualMemory = memoryStatus.ullTotalVirtual;
        ramInfo.m_FreeVirtualMemory = memoryStatus.ullAvailVirtual;
        ramInfo.m_ExtendedMemory = memoryStatus.ullAvailExtendedVirtual; // Always 0.

        return ramInfo;
    }
}