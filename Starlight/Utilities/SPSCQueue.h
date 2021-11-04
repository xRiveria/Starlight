#pragma once
#include <atomic>
#include <cassert>
#include <cstddef>
#include <memory>   // std::allocator
#include <new>      // std::hardware_destructive_interference_size
#include <stdexcept>
#include <type_traits> // std::enable_if, std::is_constructible

namespace Utilities
{
    template <typename T, typename Allocator = std::allocator<T>>
    class SPSCQueue
    {
#if defined(__cpp_if_constexpr) && defined(__cpp_lib_void_t)
        template <typename Alloc2, typename = void>
        struct has_allocate_at_least : std::false_type {};

        template <typename Alloc2>
        struct has_allocate_at_least < Alloc2, std::void_t<typename Alloc2::value_type, decltype(std::declval<Alloc2&>().allocate_at_least(size_t{})) >> : std::true_type{};
#endif
    public:
        explicit SPSCQueue(const size_t capacity, const Allocator& allocator = Allocator()) : m_Capacity(capacity), m_Allocator(allocator)
        {
            // The queue needs at least one element.
            if (m_Capacity < 1)
            {
                m_Capacity = 1;
            }

            m_Capacity++; // Allocate one extra slot to avoid false sharing on the last slot.

            // Prevent overflowing.
            if (m_Capacity > SIZE_MAX - 2 * m_SlotPadding)
            {
                m_Capacity = SIZE_MAX - 2 * m_SlotPadding;
            }

#if defined(__cpp_if_constexpr) && defined (__cpp_lib_void_t)
            // Allocate capacity size + 2 (padding for start and end to prevent false sharing).
            if constexpr (has_allocate_at_least<Allocator>::value)
            {
                auto result = allocator.allocate_at_least(m_Capacity + 2 * m_SlotPadding);
                m_Slots = result.ptr;
                m_Capacity = result.count - 2 * m_SlotPadding;
            }
            else
            {
                m_Slots = std::allocator_traits<Allocator>::allocate(m_Allocator, m_Capacity + 2 * m_SlotPadding);
            }
#else
            m_Slots = std::allocator_traits<Allocator>::allocate(m_Allocator, m_Capacity + 2 * m_SlotPadding);
#endif

            static_assert(alignof(SPSCQueue<T>) == m_CacheLineSize, "");
            static_assert(sizeof(SPSCQueue<T>) >= 3 * m_CacheLineSize, "");
            assert(reinterpret_cast<char*>(&m_ReadIndex) - reinterpret_cast<char*>(&m_WriteIndex) >= static_cast<std::ptrdiff_t>(m_CacheLineSize));
        }

        ~SPSCQueue()
        {
            while (front())
            {
                pop();
            }

            // m_Capacity + 2 * m_Padding, because the start and end of our slots are padded with the false sharing range.
            std::allocator_traits<Allocator>::deallocate(m_Allocator, m_Slots, m_Capacity + 2 * m_SlotPadding);
        }

        // Non-copyable and non-movable.
        SPSCQueue(const SPSCQueue&) = delete;
        SPSCQueue& operator=(const SPSCQueue&) = delete;

        // Enqueue an item using inplace construction. Blocks if the queue is full.
        template <typename ...Args>
        void emplace(Args&&... args) noexcept (std::is_nothrow_constructible<T, Args&&...>::value) // If it is no throw constructible, declare it to not throw exceptions.
        {
            static_assert(std::is_constructible<T, Args&&...>::value, "T must be constructible with Args&&...");
            const unsigned long long writeIndex = m_WriteIndex.load(std::memory_order_relaxed);
            unsigned long long nextWriteIndex = m_WriteIndex + 1;

            if (nextWriteIndex == m_Capacity) // Our queue is full!
            {
                nextWriteIndex = 0;
            }

            // If our queue is full...
            while (nextWriteIndex == m_ReadIndexCache)
            {
                m_ReadIndexCache = m_ReadIndex.load(std::memory_order_acquire); // Block and keep trying to load in an avaliable slot to break out.
            }

            // At this stage, we have a slot.
            new (&m_Slots[m_WriteIndex + m_SlotPadding]) T(std::forward<Args>(args)...);
            m_WriteIndex.store(nextWriteIndex, std::memory_order_release); 
        }

        // Tries to enqueue an item. Returns true on success and false if the queue is full.
        template <typename... Args>
        bool try_emplace(Args&&... args) noexcept (std::is_nothrow_constructible<T, Args&&...>::value)
        {
            static_assert(std::is_constructible<T, Args&&...>::value, "T must be constructible with Args&&...");

            const unsigned long long writeIndex = m_WriteIndex.load(std::memory_order_relaxed);
            unsigned long long nextWriteIndex = writeIndex + 1;

            // If our queue is full...
            if (nextWriteIndex == m_Capacity)
            {
                nextWriteIndex = 0;
            }

            if (nextWriteIndex == m_ReadIndexCache)
            {
                m_ReadIndexCache = m_ReadIndex.load(std::memory_order_acquire); // Make sure that our cache is accurate.
                if (nextWriteIndex == m_ReadIndexCache)
                {
                    return false;
                }
            }

            new (&m_Slots[writeIndex + m_SlotPadding]) T(std::forward<Args>(args)...);
            m_WriteIndex.store(nextWriteIndex, std::memory_order_release);
            return true;
        }

        // Enqueue an item using copy construction. Blocks if the queue is full.
        void push(const T& value) noexcept(std::is_nothrow_copy_constructible<T>::value)
        {
            static_assert(std::is_copy_constructible<T>::value, "T must be copy constructible.");
            emplace(value);
        }

        // Enqueue an item using move construction. Participates in overload resolution only if std::is_constructible is true. Blocks if queue is full otherwise.
        template <typename P, typename = typename std::enable_if<std::is_constructible<T, P&&>::value>::type>
        void push(P&& value) noexcept(std::is_nothrow_constructible<T, P&&>::value)
        {
            emplace(std::forward<P>(value));
        }

        // Try to enqueue an item using copy construction. Returns true on success and false if the queue is full.
        bool try_push(const T& value) noexcept (std::is_nothrow_copy_constructible<T>::value)
        {
            static_assert(std::is_copy_constructible<T>::value, "T must be copy constructible.");

            return try_emplace(value);
        }

        // Try to enqueue an item using move construction. Returns true on success and false if the queue is full.
        template <typename P, typename = typename std::enable_if<std::is_constructible<T, P&&>::value>::type>
        bool try_push(P&& value) noexcept(std::is_nothrow_constructible<T, P&&>::value)
        {
            return try_emplace(std::forward<P>(value));
        }

        // Returns a pointer to the front of the queue. Returns nullptr if the queue is empty.
        T* front() noexcept
        {
            const unsigned long long readIndex = m_ReadIndex.load(std::memory_order_relaxed);
            if (readIndex == m_WriteIndexCache) // If the current read index is the same as the write index, this signifies that the queue is full.
            {
                m_WriteIndexCache = m_WriteIndex.load(std::memory_order_acquire); // Hence, we load the tail index (write index) into the cached tail index. Remember that the cached tail index is used to reduce cache coherency traffic.
                if (m_WriteIndexCache == readIndex) // If the indexes still match, the queue is really empty.
                {
                    return nullptr;
                }
            }

            return &m_Slots[m_ReadIndex + m_SlotPadding];
        }

        // Dequeue first element of the queue. Invalid to call if the queue is empty.
        void pop() noexcept
        {
            static_assert(std::is_nothrow_destructible<T>::value, "T must be nothrow destructible.");

            const unsigned long long readIndex = m_ReadIndex.load(std::memory_order_relaxed); 
            assert(m_WriteIndex.load(std::memory_order_acquire) != m_ReadIndex); // Ensure that our queue isn't empty. This can happens when both the read and write index are at 0.

            m_Slots[readIndex + m_SlotPadding].~T(); // Remember that our slots are padded. 
            unsigned long long nextReadIndex = m_ReadIndex + 1;

            if (nextReadIndex == m_Capacity)
            {
                nextReadIndex = 0;
            }

            m_ReadIndex.store(nextReadIndex, std::memory_order_release);
        }

        size_t size() const noexcept
        {
            // 3 - 0
            std::ptrdiff_t difference = m_WriteIndex.load(std::memory_order_acquire) - m_ReadIndex.load(std::memory_order_acquire);

            if (difference < 0)
            {
                difference += m_Capacity;
            }

            return static_cast<size_t>(difference);
        }

        bool empty() const noexcept { return size() == 0; }
        
        // Our implementation allows for arbitrary non-power of two capacities, instead allocating an extra queue slot to indicate a full queue.
        size_t capacity() const noexcept { return m_Capacity - 1; }

    private:
#ifdef __cpp_lib_hardware_interference_size
        static constexpr size_t m_CacheLineSize = std::hardware_destructive_interference_size;
#else
        static constexpr size_t m_CacheLineSize = 64;
#endif

        // Padding to avoid false sharing between slots and adjacent allocations.
        static constexpr size_t m_SlotPadding = (m_CacheLineSize - 1) / sizeof(T) + 1;

    private:
        size_t m_Capacity;
        T* m_Slots;

#if defined(__has_cpp_attribute) && __has_cpp_attribute(no_unique_address)
        Allocator m_Allocator [[no_unique_address]];
#else
        Allocator m_Allocator;
#endif

        // Align to cache line size in order to avoid false sharing.
        alignas(m_CacheLineSize) std::atomic<size_t> m_WriteIndex = { 0 };  // Producer works with the tail.
        alignas(m_CacheLineSize) size_t m_ReadIndexCache = 0;
        alignas(m_CacheLineSize) std::atomic<size_t> m_ReadIndex = { 0 };   // Consumer works with the head.
        alignas(m_CacheLineSize) size_t m_WriteIndexCache = 0;

        // Padding to avoid adjacent allocations to share cache line with m_WriteIndexCache.
        char m_Padding[m_CacheLineSize - sizeof(m_WriteIndexCache)];
    };
}