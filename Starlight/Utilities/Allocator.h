#pragma once
#include <atomic>
#include <cassert>
#include <cstddef>
#include <memory> // std::allocator
#include <new>    // std::hardware_destructive_interference_size
#include <stdexcept>
#include <type_traits> // std::enable_if, std::is_*_constructible

struct Default
{
    int foo() const { return 1; }
};

template <typename Alloc2, typename = void>
struct has_allocate_at_least : std::false_type {};

template <typename Alloc2>
struct has_allocate_at_least<Alloc2, std::void_t<typename Alloc2::value_type, decltype(std::declval<Alloc2&>().allocate_at_least(size_t{})) >> : std::true_type{};

template <typename T, typename Allocator = std::allocator<T>>
class Vector
{
    // Allocates bytes of uninitialized storage. An array of type is created, but none of its elements are constructed.
public:
    Vector(size_t capacity, const Allocator& allocator = Allocator()) : m_Allocator(allocator)
    {
        if constexpr (has_allocate_at_least<Allocator>::value)
        {
            auto result = allocator.allocate_at_least(64);
        }

        decltype(Default().foo()) derp = 1; // Declared value type of nl is int. 
        std::cout << 2;
        // Makes it possible to use member functions in decltype expressions without the need to go through constructors. 
        decltype(std::declval<Default>().foo()) derp2 = derp;

        m_Memory = std::allocator_traits<Allocator>::allocate(m_Allocator, capacity * sizeof(4));
        new (&m_Memory[4]) T(6);
        new (&m_Memory[3]) T(3);
        std::cout << m_Memory[4] << "\n";

        std::ptrdiff_t diff = m_Memory[4] - m_Memory[3];
        std::cout << diff << "\n";
    }

    // std::void_t is a utilty megafunction that maps a sequence of types to the type void.
    /*
        This metafunction is used in template metaprogramming to detect ill-formed types in SFINAE context.
    */

    // Primary template handles types that have no tested ::type member.
    template <typename T, typename = void>
    struct HasTypeMember : std::false_type { };

    // Specialization recognizes types that do have a nested ::type member.
    template <typename T>
    struct HasTypeMember<T, std::void_t<typename T::type>> : std::true_type { };

private:
    Allocator m_Allocator;

    T* m_Memory;
    T* m_StuffHere;
};