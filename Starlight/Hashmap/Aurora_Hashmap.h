#pragma once
#include <cassert>
#include <cstring>
#include <string>
#include <limits>       // For numeric_limits.
#include <algorithm>    // For swap(), etc.
#include <iterator>     // For iterator tags.
#include <functional>   // For equal_to<>, select1st<>, std::unary_function, etc.
#include <memory>       // For alloc, uninitialized_copy, uninitialized_fill.
#include <cstdlib>      // For malloc/realloc/free.
#include <cstddef>      // For ptrdiff_t.
#include <new>          // For placement new.
#include <stdexcept>    // For length_error.
#include <utility>      // For pair<>.
#include <cstdio>
#include <iosfwd>
#include <ios>

// 

template <typename T, typename Allocator>
class Aurora_Group
{
public:
    // Basic Type
    typedef T                                               value_type;
    typedef Allocator                                       allocator_type;
    typedef value_type&                                     reference;
    typedef const value_type&                               const_reference;
    typedef value_type*                                     pointer;
    typedef const value_type*                               const_pointer;

    typedef uint8_t                                         size_t;     // Max # of buckets.

    // These are our special iterators that go over non-empty buckets in a group. These aren't const-only because you can change non-empty buckets.
    typedef pointer                                         non_empty_iterator;
    typedef const_pointer                                   const_non_empty_iterator;
    typedef std::reverse_iterator<non_empty_iterator>       reverse_non_empty_iterator;


};

template <typename T, typename Allocator>
class Aurora_Table
{
public:
    typedef T                                           value_type;
    typedef Allocator                                   allocator_type;
    typedef Aurora_Group<value_type, allocator_type>    group_type;
};

template <typename Value, typename Key, typename HashFunction, typename ExtractKey, typename SetKey, typename EqualKey, typename Allocator>
class Aurora_HashTable
{
public:
    typedef Key                                              key_type;
    typedef Value                                            value_type;
    typedef HashFunction                                     hasher; 
    typedef EqualKey                                         key_equal;
    typedef Allocator                                        allocator_type;

    typedef typename allocator_type::size_type               size_type;
    typedef typename allocator_type::difference_type         difference_type;
    typedef value_type&                                      reference;
    typedef const value_type&                                const_reference;
    typedef value_type*                                      pointer;
    typedef const value_type*                                const_pointer;

    // Table is the main storage class.


};