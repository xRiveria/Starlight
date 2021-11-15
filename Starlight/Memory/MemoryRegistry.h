#pragma once
#include <vector>
#include <unordered_map>

namespace Memory
{
    enum class MemoryPoolType
    {
        MemoryPoolType_General,
        MemoryPoolType_Objects,
        MemoryPoolType_Test,
        MemoryPoolType_Unknown
    };

    struct AllocationMetrics
    {
        AllocationMetrics() = default;
        AllocationMetrics(MemoryPoolType allocationType, size_t allocationSize) : m_AllocationType(allocationType), m_AllocationSize(allocationSize) { }

        size_t m_AllocationSize = 0;
        size_t m_AllocationFreed = 0;

        size_t GetCurrentUsage() { return m_AllocationSize - m_AllocationFreed; }

        MemoryPoolType m_AllocationType = MemoryPoolType::MemoryPoolType_Unknown;
    };

    class MemoryPoolRegistry
    {
    public:
        static MemoryPoolRegistry& GetInstance()
        {
            static MemoryPoolRegistry registryInstance;
            return registryInstance;
        }

        uint32_t GetMemoryPoolUsage(MemoryPoolType poolType) { return m_AllocationMetrics[poolType].GetCurrentUsage(); }

        void* AllocateMemoryBlock(MemoryPoolType poolType, std::size_t blockSize)
        {
            AllocationMetrics newMetrics = AllocationMetrics(poolType, blockSize);
            m_AllocationMetrics[poolType] = newMetrics;
            return malloc(blockSize);
        }

    private:
        MemoryPoolRegistry() {};

    private:
        std::unordered_map<MemoryPoolType, AllocationMetrics> m_AllocationMetrics;

    };
}