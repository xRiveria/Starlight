#pragma once
#include <string>
#include "TypeDescriptor.h"
#include "Any.h"

namespace Reflect
{
    class DataMember
    {
    public:
        std::string GetName() const { return m_Name; }
        const TypeDescriptor* GetParent() const { return m_Parent; }
        const TypeDescriptor* GetType() const { return m_Type; }

        virtual void Set(AnyReference objectReference, const Any value) = 0;
        virtual void Get(Any object) = 0;

    protected:
        DataMember(const std::string& name, const TypeDescriptor* type, const TypeDescriptor* parent) : m_Name(name), m_Type(type), m_Parent(parent) { }

    private:
        std::string m_Name;
        const TypeDescriptor* m_Type;   // Type of the data member.
        const TypeDescriptor* m_Parent; // Type of the data member's class.
    };

    template <typename Class, typename Type>
    class PointerDataMember : public DataMember
    {
    public:
        PointerDataMember(Type Class::*dataMemberPointer, const std::string name) : DataMember(name, Details::Resolve<Type>(), Details::Resolve<Class>()),
            m_DataMemberPointer(dataMemberPointer) { }

        void Set(AnyReference objectReference, const Any value) override
        {
            SetImplementation(objectReference, value, std::is_const<Type>());
        }

        Any Get(Any object) override
        {
            Class* object = object.TryCast<Class>();

            if (!object)
            {
                std::abort();
            }

            return object->*m_DataMemberPointer;
        }

    private:
        // Use tag dispatch.
        void SetImplementation(Any object, const Any value, std::false_type)
        {
            Class* object = object.TryCast<Class>(); // Pointers to member of base class can be used with derived class.

            if (!object)
            {
                std::abort();
            }

            const Type* casted = nullptr;
            Any val;
            if (casted = value.TryCast<Type>(); !casted)
            {
                val = value.TryConvert<Type>();
                casted = val.TryCast<Type>();
            }

            if (!casted)
            {
                std::abort();
            }

            object->*m_DataMemberPointer = *casted;
        }

    private:
        Type Class::*m_DataMemberPointer;

    };

    // Helper meta function to get info about functions passed as auto non type parameters (C++17).
    template <typename>
    struct FunctionHelper;

    template <typename Return, typename ...Args>
    struct FunctionHelper<Return(Args...)>
    {
        using ReturnType = Return;
        using ParameterTypes = std::tuple<Args...>;
    };

    template <typename Class, typename Return, typename ...Args>
    FunctionHelper<Return(Args...)> ToFunctionHelper(Return(Class:*)(Args...));
    
    template <typename Class, typename Return, typename ...Args>
    FunctionHelper<Return(Args...)> ToFunctionHelper(Return(Class:*)(Args...) const);

    template <typename Return, typename... Args>
    FunctionHelper<Return(Args...)> ToFunctionHelper(Return(*)(Args...));

    template <auto Setter, auto Getter, typename Class>
    class SetGetDataMember : public DataMember
    {
    private:
        using MemberType = Details::RawType<typename decltype(ToFunctionHelper(Getter))::ReturnType>;

    public:
        SetGetDataMember(const std::string name) : DataMember(name, Details::Resolve<MemberType>(), Details::Resolve<Class>()) { }

        void Set(AnyReference objectReference, const Any value) override
        {
            Any a = objectReference;
            Class* object = a.TryCast<Class>();

            if (!object) 
            { 
                std::abort();
            }

            const MemberType* casted = nullptr;
            Any val;
            if (casted = value.TryCast<MemberType>(); !casted)
            {
                val = value.TryConvert<MemberType>();
                casted = val.TryCast<MemberType>(); 
            }

            if (!casted)
            {
                std::abort();
            }

            if constexpr (std::is_member_function_pointer_v<decltype(Setter)>)
            {
                (object->*Setter)(*casted);
            }
            else
            {
                static_assert(std::is_function_v<std::remove_pointer_t<decltype(Setter)>>);

                Setter(*object, *casted);
            }
        }

        Any Get(Any object) override
        {
            Class* object = object.TryCast<Class>();

            if (!object) { std::abort(); }

            if constexpr (std::is_member_function_pointer_v<decltype(Setter)>)
            {
                return (object->*Getter)();
            }
            else
            {
                static_assert(std::is_function_v<std::remove_pointer_t<decltype(Getter)>>);

                return Getter(*object);
            }
        }
    };

}