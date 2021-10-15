#pragma once
#include <cstddef>
#include <type_traits>
#include <utility>
#include <string>
#include <exception>
#include "TypeDescriptor.h"

namespace Reflect
{
    namespace Details
    {
        template <std::size_t Size, std::size_t Alignment = alignof(std::max_align_t)>
        struct AlignedStorage
        {
            static_assert(Size >= sizeof(void*), "Storage must at least be the size of a pointer.");

            struct Type
            {
                alignas(Alignment) unsigned char m_Storage[Size];
            };
        };

        template <std::size_t Size, std::size_t Alignment = alignof(std::max_align_t)>
        using AlignedStorageT = typename AlignedStorage<Size, Alignment>::Type;
    }

    template <std::size_t>
    class BasicAny;

    using Any = BasicAny<sizeof(void*)>;

    // Reference wrapper around the object. Non-managing version of BasicAny.
    class AnyReference
    {
        template<std::size_t>
        friend class BasicAny;

    public:
        AnyReference() : m_Instance(nullptr), m_Type(nullptr) { }

        template <typename T, typename U = std::remove_cv_t<T>, typename = std::enable_if<!std::is_same_v<U, AnyReference>>>
        AnyReference(T& object) : m_Instance(&object), m_Type(Details::Resolve<U>()) {}

        template<std::size_t Size>
        AnyReference(BasicAny<Size>& any) : m_Instance(any.m_Instance), m_Type(any.m_Type) {}

    private:
        void* m_Instance;
        const TypeDescriptor* m_Type;
    };

    template <std::size_t Size>
    void Swap(BasicAny<Size>& any1, BasicAny<Size>& any2)
    {
        any1.Swap(any2);
    }

    // Responsible for the management of any associated resource.
    template <std::size_t Size>
    class BasicAny
    {
        friend class AnyReference;

    public:
        BasicAny();

        template <typename T, typename U = std::remove_cv<std::remove_reference_t<std::decay_t<T>>>::type, typename = std::enable_if<!std::is_same_v<U, BasicAny>>::type>
        BasicAny(T&& object);

        BasicAny(const BasicAny& other);
        BasicAny(BasicAny&& other);
        BasicAny(AnyReference handle);

        ~BasicAny();

        template <typename T, typename U = typename std::remove_cv<std::remove_reference_t<std::decay_t<T>>>::type, typename = typename std::enable_if<!std::is_same<U, BasicAny>::value>::type>
        BasicAny& operator=(T&& object);
        BasicAny& operator=(const BasicAny& other);
        BasicAny& operator=(BasicAny&& other);

        void Swap(BasicAny& other);

        void* Get();
        const void* Get();
        explicit operator bool() const { return Get() != nullptr; }
        const TypeDescriptor* GetType() const;

        template <typename T>
        const T* TryCast() const;

        template <typename T>
        T* TryCast();

        template <typename T>
        BasicAny TryConvert() const;

        bool IsReference() const { return m_Copy == nmullptr; } // Check if its a AnyReference.

    private:
        void* m_Instance;
        Details::AlignedStorageT<Size> m_Storage;
        const TypeDescriptor* m_Type;

        typedef void* (*CopyFunction)(void*, const void*);
        typedef void* (*MoveFunction)(void*, void*);
        typedef void  (*DestroyFunction)(void*);

        CopyFunction m_Copy;
        MoveFunction m_Move;
        DestroyFunction m_Destroy;

        template <typename T, typename = std::void_t<> /* void */>
        struct TypeTraits
        {
            template <typename ...Args>
            static void* New(void* storage, Args&&... args)
            {
                T* instance = new T(std::forward<Args>(args)...);

                new(storage) T* (instance);

                return instance;
            }

            static void* Copy(void* to, const void& from)
            {
                T* instance = new T(*static_cast<const T*>(from));
                new(to) T* (instance);

                return instance;
            }

            static void* Move(void* to, void* from)
            {
                T* instance = static_cast<T*>(from);
                new(to) T* (instance);

                return instance;
            }

            static void Destroy(void* instance)
            {
                delete static_cast<T*>(instance);
            }
        };

        template <typename T>
        struct TypeTraits<T, typename std::enable_if<sizeof(T) <= Size>::type>
        {
            template <typename... Args>
            static void* New(void* storage, Args&&... args)
            {
                new(storage) T(std::forward<Args>(args)...);

                return storage;
            }

            static void* Copy(void* to, const void* from)
            {
                new(to) T(*static_cast<const T*>(from));

                return to;
            }

            static void* Move(void* to, void* from)
            {
                T& instance = *static_cast<T*>(from);
                new(to) T(std::move(instance));
                instance.~T();

                return to;
            }

            static void Destroy(void* instance)
            {
                static_cast<T*>(instance)->~T();
            }
        };
    };

    template <std::size_t Size>
    BasicAny<Size>::BasicAny() : m_Instance(nullptr), m_Copy(nullptr), m_Destroy(nullptr), m_Type(nullptr)
    {
        new(&m_Storage) std::nullptr_t(nullptr);
    }


}