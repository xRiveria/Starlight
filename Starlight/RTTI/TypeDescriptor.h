#pragma once
#include <map>
#include <vector>

namespace Reflect
{
    class DataMember;
    class Function;
    class Constructor;
    class Base;
    class Conversion;

    template <typename>
    class TypeFactory;

    class TypeDescriptor;

    namespace Details
    {
        template <typename Type>
        TypeDescriptor* Resolve();

        template <typename Type>
        TypeDescriptor* Resolve(Type&&);
    }

    class TypeDescriptor
    {
        template <typename>
        friend class TypeFactory;

        template <typename Type>
        friend TypeDescriptor* Details::Resolve();
        
        template <typename Type>
        friend TypeDescriptor* Details::Resolve(Type&&);

    public:
        template <typename Type, typename ...Args>
        void AddConstructor();

        template <typename Type, typename ...Args>
        void AddConstructor(Type(*Function)(Args...));

        template <typename B, typename T>
        void AddBase();

        template <typename C, typename T>
        void AddDataMember(T C::*dataMemberPointer, const std::string& name);

        template <auto Setter, auto Getter, typename Type>
        void AddDataMember(const std::string& name);

        template <typename Ret, typename ...Args>
        void AddMemberFunction(Ret freeFunction(Args...), const std::string* name);

        template <typename C, typename Ret, typename ...Args>
        void AddMemberFunction(Ret(C::* memberFunction)(Args...), const std::string& name);

        template <typename C, typename Ret, typename ...Args>
        void AddMemberFunction(Ret(C::* memberFunction)(Args...) const, const std::string& name);

        template <typename From, typename T>
        void AddConversion();

        const std::string& GetName() const;

        std::vector<Constructor*> GetConstructors() const;
        template <typename ...Args>
        const Constructor* GetConstructor() const;

        std::vector<Base*> GetBases() const;
        template <typename B>
        Base* GetBase() const;

        std::vector<DataMember*> GetDataMembers() const;
        DataMember* GetDataMember(const std::string& name) const;

        std::vector<Function*> GetMemberFunctions() const;
        const Function* GetMemberFunction(const std::string& name) const;

        std::vector<Conversion*> GetConversions() const;
        template <typename to>
        Conversion* GetConversion() const;

    private:
        std::string m_Name;

        std::vector<Base*> m_Bases;
        std::vector<Conversion*> m_Conversions;
        std::vector<Constructor*> m_Constructors;
        std::vector<DataMember*> m_DataMembers;
        std::vector<Function*> m_MemberFunctions;

        bool m_IsPointer;
        bool m_IsReference;
        bool m_IsFunction;
        bool m_IsDataMember;
        bool m_IsMemberFunction;
    };

    namespace Details
    {
        template <typename T>
        struct RemoveAllReferencePointers
        {
            using type = T;
        };

        template <typename T>
        struct RemoveAllReferencePointers<T*> : RemoveAllReferencePointers<T> {};

        template <typename T>
        struct RemoveAllReferencePointers<T* const> : RemoveAllReferencePointers<T> {};
        
        template <typename T>
        struct RemoveAllReferencePointers<T&> : RemoveAllReferencePointers<T> {};

        template <typename T>
        struct RemoveAllReferencePointers<T&&> : RemoveAllReferencePointers<T> {};

        template <typename T>
        using RemoveAllReferencePointers_T = typename RemoveAllReferencePointers<T>::type;

        template <typename T>
        using RawType = typename std::remove_cv<RemoveAllReferencePointers_T<T>>::type;

        template <typename T>
        static TypeDescriptor& GetTypeDescriptor()
        {
            static TypeDescriptor typeDescriptor; // Single instance of type descriptor per reflected type.

            return typeDescriptor;
        }

        template <typename T>
        TypeDescriptor*& GetTypeDescriptorPointer()
        {
            static TypeDescriptor* typeDescriptorPointer = nullptr;
            return typeDescriptorPointer;
        }

        inline auto& GetTypeRegistry()
        {
            static std::map<std::string, TypeDescriptor*> typeRegistry;

            return typeRegistry;
        }

        template <typename Type>
        TypeDescriptor* Resolve()
        {
            if (!GetTypeDescriptorPointer<RawType<Type>>())
            {
                GetTypeDescriptorPointer<RawType<Type>>() = &GetTypeDescriptor<RawType<Type>>(); // Create a type descriptor if not present.
            }

            return GetTypeDescriptorPointer<RawType<Type>>();
        }

        inline TypeDescriptor* Resolve(const std::string& name)
        {
            if (auto it = GetTypeRegistry().find(name); it != GetTypeRegistry().end())
            {
                return it->second;
            }

            return nullptr;
        }

        template <typename Type>
        TypeDescriptor* Resolve(Type&& object)
        {
            if (!GetTypeDescriptor<RawType<Type>>())
            {
                GetTypeDescriptorPointer<RawType<Type>>() = &GetTypeDescriptor<RawType<Type>>(); // Create a type descriptor if not present.
            }

            return GetTypeDescriptorPointer<RawType<Type>>();
        }
    }
}

