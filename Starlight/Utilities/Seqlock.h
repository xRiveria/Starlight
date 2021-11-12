#pragma once
#include <atomic>
#include <type_traits>

#ifndef NDEBUG
#define SEQLOCK_NOINLINE
#else
#define SEGLOCK_NOINLINE
#endif

namespace Utilities
{
    template <typename T>
    class Seqlock
    {
    public:
        static_assert(std::is_nothrow_copy_assignable<T>::value, "T must satisfy is_no_throw_copy_assignable.");
        static_assert(std::is_trivially_copy_assignable<T>::value, "T must satisfy is trivially_copy_assignable.");

        Seqlock() : m_Sequence(0)
        {

        }

        // Load a value (can be called from multiple threads).
        SEQLOCK_NOINLINE T Load() const noexcept
        {
            T copy;
            std::size_t sequence0, sequence1;

            do
            {
                sequence0 = m_Sequence.load(std::memory_order_acquire);
                std::atomic_signal_fence(std::memory_order_acq_rel);    // Writes in other threads are visible before the modification, modification is visible in other threads.
                copy = m_Value;
                std::atomic_signal_fence(std::memory_order_acq_rel);
                sequence1 = m_Sequence.load(std::memory_order_acquire);
            }

            while (sequence0 != sequence1 || sequence0 & 1);

            return copy;
        }

        // Can only store a value (can only be called from a single thread).
        SEQLOCK_NOINLINE void Store(const T& desired) noexcept
        {
            std::size_t sequence0 = m_Sequence.load(std::memory_order_relaxed);
            m_Sequence.store(sequence0 + 1, std::memory_order_release);
            std::atomic_signal_fence(std::memory_order_acq_rel);
            m_Value = desired;
            std::atomic_signal_fence(std::memory_order_acq_rel);
            m_Sequence.store(sequence0 + 2, std::memory_order_release);
        }

    private:
        static const std::size_t m_FalseSharingRange = 128;

        // Align to prevent false sharing with adjacent data.
        alignas(m_FalseSharingRange) T m_Value;
        std::atomic<std::size_t> m_Sequence;

        // Padding to prevent false sharing with adjacent data.
        char m_Padding[m_FalseSharingRange - ((sizeof(m_Value) + sizeof(m_Sequence)) % m_FalseSharingRange)];

        static_assert(((sizeof(m_Value) + sizeof(m_Sequence) + sizeof(m_Padding)) % m_FalseSharingRange) == 0, "sizeof(Seqlock<T>) should be a multiple of m_FalseSharingRange");
    };
}
