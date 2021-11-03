#pragma once
#include <atomic>
#include <cassert>
#include <cstddef>
#include <limits>
#include <memory>
#include <new>
#include <stdexcept>

// Multi Producer, Multi Consumer Concurrent Queue.
#ifndef __cpp_aligned_new
#ifdef _WIN32
#include <malloc.h>  // Aligned Malloc
#else
#include <stdlib.h>  // posix_memalign
#endif
#endif

/*
    - Cache line in modern processors are typically 64 bytes. Therefore, we will ensure that each of our atomics are 64 byte aligned to prevent false sharing.
    - This will guarentee that our atomics are on different cache lines/blocks.
    - Compare this to 4 atomics of integers - Said integers will likely be allocated on the same cache line. Hence, when a thread grabs its dedicated atomic, it grabs all 4.

    Remember that we will always allocate one extra slot to prevent false sharing on the last slot.
*/

namespace Utilities
{
#ifdef __cpp_lib_hardware_interference_size
    static constexpr size_t hardwareInterferenceSize = std::hardware_destructive_interference_size;
#else
    static constexpr size_t hardwareInterferenceSize = 64;
#endif

    // Dynamic memory allocation for over-aligned data.
#if defined (__cpp_aligned_new)
    template <typename T>
    using AlignedAllocator = std::allocator<T>;
#else
    template <typename T>
    struct AlignedAllocator
    {
        using ValueType = T;

        T* allocate(std::size_t n)
        {
            if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            {
                throw std::bad_array_new_length();
            }

            auto* ptr = static_cast<T*>(_aligned_malloc(sizeof(T) * n, alignof(T)));
            if (ptr == nullptr)
            {
                throw std::bad_alloc();
            }

            return ptr;
        }

        void deallocate(T* ptr, std::size_t)
        {
            _aligned_free(ptr);
        }
    };
#endif

    template <typename T>
    struct Slot
    {
        ~Slot() noexcept
        {
            if (m_Turn & 1)
            {
                destroy();
            }
        }

        template <typename ...Args>
        void construct(Args&&... args) noexcept
        {
            static_assert(std::is_nothrow_constructible<T, Args&&...>::value, "T must be nothrow constructible with Args&&...");
            new (&m_Storage) T(std::forward<Args>(args)...);
        }

        void destroy() noexcept
        {
            static_assert(std::is_nothrow_destructible<T>::value, "T must be a nothrow destructible with Args&&...");
            reinterpret_cast<T*>(&m_Storage)->~T();
        }

        T&& move() noexcept
        {
            return reinterpret_cast<T&&>(m_Storage);
        }

        // Align to avoid false sharing between adjacent slots.
        alignas(hardwareInterferenceSize) std::atomic<size_t> m_Turn = { 0 };
        typename std::aligned_storage<sizeof(T), alignof(T)>::type m_Storage;
    };

    template <typename T, typename Allocator = AlignedAllocator<Slot<T>>>
    class Queue
    {
    public:
        explicit Queue(const size_t capacity, const Allocator& allocator = Allocator()) : m_Capacity(capacity), m_Allocator(allocator), m_Head(0), m_Tail(0)
        {
            if (m_Capacity < 1)
            {
                throw std::invalid_argument("Queue capacity less than 1!");
            }

            // Always allocates one extra slot to prevent false sharing on the last slot.
            m_Slots = m_Allocator.allocate(m_Capacity + 1);

            // Allocators are not required to honor alignment for over-aligned types. Hence, we verify the alignment ourselves here.
            if (reinterpret_cast<size_t>(m_Slots) % alignof(Slot<T>) != 0)
            {
                m_Allocator.deallocate(m_Slots, m_Capacity + 1);
                throw std::bad_alloc();
            }

            // Allocate
            for (size_t i = 0; i < m_Capacity; ++i)
            {
                new (&m_Slots[i]) Slot<T>();
            }

            // 64 bytes.
            static_assert(alignof(Slot<T>) == hardwareInterferenceSize, "Slot must be aligned to cache line boundary to prevent false sharing.");
        }

        ~Queue() noexcept
        {
            for (size_t i = 0; i < m_Capacity; ++i)
            {
                m_Slots[i].~Slot();
            }

            m_Allocator.deallocate(m_Slots, m_Capacity + 1);
        }

        // Non-copyable and non-movable.
        Queue(const Queue&) = delete;
        Queue& operator=(const Queue&) = delete;

        // Addition of an item into our queue. Blocks if the queue is full.
        template <typename ...Args>
        void emplace(Args&&... args) noexcept
        {
            // Ensure that our constructor doesn't throw runtime exceptions.
            static_assert(std::is_nothrow_constructible<T, Args&&...>::value, "T must be nothrow constructible with Args&&...");

            const unsigned long long head = m_Head.fetch_add(1); // Current Head + 1
            auto& slot = m_Slots[Index(head)]; // Grabs a slot with the index of our current head. 

            // Wait for our turn to write slot.
            while (Turn(head) * 2 != slot.m_Turn.load(std::memory_order_acquire))
                ;

            // Constructs our slot.
            slot.construct(std::forward<Args>(args)...);

            // Set turn to turn + 1 to inform writers that we are done reading.
            slot.m_Turn.store(Turn(head) * 2 + 1, std::memory_order_release);
        }

        // Try to enqueue an item using inplace construction. Returns true on success and false if the queue is full. 
        template <typename... Args>
        bool try_emplace(Args&&... args) noexcept
        {
            static_assert(std::is_nothrow_constructible<T, Args&&...>::value, "T must be nothrow constructible with Args&&...");
            unsigned long long head = m_Head.load(std::memory_order_acquire);

            for (;;)
            {
                auto& slot = m_Slots[Index(head)];
                if (Turn(head) * 2 == slot.m_Turn.load(std::memory_order_acquire))
                {
                    if (m_Head.compare_exchange_strong(head, head + 1))
                    {
                        slot.construct(std::forward<Args>(args)...);
                        slot.m_Turn.store(Turn(head) * 2 + 1, std::memory_order_acquire);
                        return true;
                    }
                }
                else // Block
                {
                    const auto previousHead = head;
                    head = m_Head.load(std::memory_order_acquire);

                    if (head == previousHead)
                    {
                        return false;
                    }
                }
            }
        }

        template <typename P, typename = typename std::enable_if<std::is_nothrow_constructible<T, P&&>::value>::type>
        void push(P&& value) noexcept
        {
            emplace(std::forward<P>(value));
        }

        bool try_push(const T& value) noexcept
        {
            static_assert(std::is_nothrow_copy_constructible<T>::Value, "T must be a nothrow copy constructible.");
            return try_emplace(value);
        }

        template <typename P, typename = typename std::enable_if<std::is_nothrow_constructible<T, P&&>::value>::type>
        bool try_push(P&& value) noexcept
        {
            return try_emplace(std::forward<P>(value));
        }

        void pop(T& value) noexcept
        {
            // Acquire a read ticket from the tail. 
            const unsigned long long tail = m_Tail.fetch_add(1);
            auto& slot = m_Slots[Index(tail)];

            // Wait for our turn to read the slot. 
            while (Turn(tail) * 2 + 1 != slot.m_Turn.load(std::memory_order_acquire))
                ;
            
            value = slot.move();
            slot.destroy();

            // Inform the writers that we are done reading.
            slot.m_Turn.store(Turn(tail) * 2 + 2, std::memory_order_release);
        }

        
        bool try_pop(T& value) noexcept
        {
            unsigned long long tail = m_Tail.load(std::memory_order_acquire);
            for (;;)
            {
                auto& slot = m_Slots[Index(tail)];
                if (Turn(tail) * 2 + 1 == slot.m_Turn().load(std::memory_order_acquire))
                {
                    if (m_Tail.compare_exchange_strong(tail, tail + 1))
                    {
                        value = slot.move();
                        slot.destroy();
                        slot.m_Turn.store(Turn(tail) * 2 + 2, std::memory_order_release);
                        return true;
                    }
                }
                else
                {
                    const auto previousTail = tail;
                    tail = m_Tail.load(std::memory_order_acquire);
                    if (tail == previousTail)
                    {
                        return false;
                    }
                }
            }
        }

    private:
        constexpr size_t Index(size_t i) const noexcept { return i % m_Capacity; }
        constexpr size_t Turn(size_t i) const noexcept { return i / m_Capacity; }

    private:
        const size_t m_Capacity;
        Slot<T>* m_Slots;

        // Checks for the presence of an attribute name, such as "nodiscard", "noreturn", "no_unique_address" etc.
        // All objects specify a size oif at least 1 even if the type is empty. However, no_unique_address indicates that the data member need not have an address distinct from other non-static data members.
        // This means that the compiler may optimise it to occupy no space, just like if it were an empty base. If the member is not empty, any tail padding in it may be reused to store other data members.
#if defined (__has_cpp_attribute) && __has_cpp_attribute(no_unique_address) 
        Allocator m_Allocator_ [[no_unique_address]];
#else
        Allocator m_Allocator;
#endif

        // Align to avoid false sharing between head and tail.
        alignas(hardwareInterferenceSize) std::atomic<size_t> m_Head;
        alignas(hardwareInterferenceSize) std::atomic<size_t> m_Tail;

        static_assert(std::is_nothrow_copy_assignable<T>::value || std::is_nothrow_move_assignable<T>::value, "T must be nothrow copy or move assignable.");
        static_assert(std::is_nothrow_destructible<T>::value, "T must be nothrow destructible.");
    };

    template <typename T, typename Allocator = Utilities::AlignedAllocator<Utilities::Slot<T>>>
    using MPMCQueue = Utilities::Queue<T, Allocator>;
}

