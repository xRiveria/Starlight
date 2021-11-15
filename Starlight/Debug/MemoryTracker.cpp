#include "MemoryTracker.h"
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstddef>

#undef new

// Global flags set by our macros.
bool g_TraceFlag = true;
bool g_ActiveFlag = true;

namespace Debug
{
    namespace Memory
    {
        // Memory map entry type.
        struct MemoryAllocationInfo
        {
            void* m_Ptr;
            const char* m_File;
            long m_Line;
        };

        // Memory map data.
        const size_t g_MaxPtrCount = 10000u;
        MemoryAllocationInfo g_MemoryMappings[g_MaxPtrCount];
        size_t g_CurrentPtrCount = 0; // Always set to the next avaliable slot.

        // Searches the map for an address. 
        int FindPtr(void* ptr)
        {
            for (size_t i = 0; i < g_CurrentPtrCount; ++i)
            {
                if (g_MemoryMappings[i].m_Ptr == ptr)
                {
                    return i;
                }
            }

            return -1;
        }

        void DeletePtr(void* ptr)
        {
            int position = FindPtr(ptr);
            assert(position >= 0);

            // Remove pointer from the map.
            for (size_t i = position; i < g_CurrentPtrCount - 1; ++i)
            {
                g_MemoryMappings[i] = g_MemoryMappings[i + 1];
                --g_CurrentPtrCount;
            }
        }

        // Inspects program to see if any memory leaks occured on shutdown and dumps output on termination.
        struct Sentinel
        {
            ~Sentinel()
            {
                if (g_CurrentPtrCount > 0)
                {
                    printf("Leaked memory at: \n");
                    for (size_t i = 0; i < g_CurrentPtrCount; ++i)
                    {
                        printf("\%p (File: %s, Line %ld)\n", g_MemoryMappings[i].m_Ptr, g_MemoryMappings[i].m_File, g_MemoryMappings[i].m_Line);
                    }
                }
                else
                {
                    printf("No user memory leaks.\n");
                }
            }
        };

        // Static dummy object.
        Sentinel g_MemorySentinel;
    }
}

// Overload scalar new.
void* operator new(size_t size, const char* file, long line)
{
    void* ptr = malloc(size);
    if (g_ActiveFlag)
    {
        if (Debug::Memory::g_CurrentPtrCount == Debug::Memory::g_MaxPtrCount)
        {
            printf("Max allocations reached. Consider increasing g_MaxPtrCount.\n");
            exit(1);
        }

        Debug::Memory::g_MemoryMappings[Debug::Memory::g_CurrentPtrCount].m_Ptr = ptr;
        Debug::Memory::g_MemoryMappings[Debug::Memory::g_CurrentPtrCount].m_File = file;
        Debug::Memory::g_MemoryMappings[Debug::Memory::g_CurrentPtrCount].m_Line = line;
        ++Debug::Memory::g_CurrentPtrCount;
    }

    if (g_TraceFlag)
    {
        printf("Allocated %u bytes at address %p. (File: %s, Line: %ld)\n", size, ptr, file, line);
    }

    return ptr;
}

// Overload array new.
void* operator new[](size_t size, const char* file, long line)
{
    return operator new(size, file, line);
}

// Override scalar delete.
void operator delete(void* ptr)
{
    if (Debug::Memory::FindPtr(ptr) >= 0)
    {
        free(ptr);
        assert(Debug::Memory::g_CurrentPtrCount > 0);
        Debug::Memory::DeletePtr(ptr);

        if (g_TraceFlag)
        {
            printf("Deleted memory at address %p.\n", ptr);
        }
        else if (!ptr && g_ActiveFlag)
        {
            printf("Attempt to delete unknown pointer: %p\n", ptr);
        }
    }
}

// Override array delete.
void operator delete[](void* ptr)
{
    operator delete(ptr);
}