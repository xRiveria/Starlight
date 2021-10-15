#pragma once
#include <string>
#include <vector>
#include <tuple>
#include "TypeDescriptor.h"
#include "Any.h"

namespace Reflect
{
    class Function
    {
    public:
        std::string GetName() const { return m_Name; }
        const TypeDescriptor* GetParent() const { return m_Parent; }

        Any Invoke(AnyReference object, std::vector<Any>& args) const
        {
            if (args.size() == m_ParameterTypes.size())
            {
                return InvokeImplementation(object, args);
            }

            return Any();
        }

        template <typename ...Args>
        Any Invoke(AnyReference object, Args&& ...args) const
        {
            if (sizeof...(Args) == m_ParameterTypes.size())
            {
                std::vector<Any> anyArguments { Any(std::forward<Args>(args))... };
                return InvokeImplementation(object, anyArguments);
            }

            return Any();
        }

        const TypeDescriptor* GetReturnType() const
        {
            return m_ReturnType;
        }

        std::vector<const TypeDescriptor*> GetParameterTypes() const
        {
            return m_ParameterTypes;
        }

        const TypeDescriptor* GetParameterType(size_t index) const
        {
            return m_ParameterTypes[index];
        }

        std::size_t GetParameterCount() const
        {
            return m_ParameterTypes.size();
        }

    protected:
        Function(const std::string& name, const TypeDescriptor* parent, const TypeDescriptor* returnType, const std::vector<const TypeDescriptor*> parameterTypes)
            : m_Name(name), m_Parent(parent), m_ReturnType(returnType), m_ParameterTypes(parameterTypes) { }

    protected:
        const TypeDescriptor* m_ReturnType;
        std::vector<const TypeDescriptor*>  m_ParameterTypes;

    private: 
        virtual Any InvokeImplementation(Any object, std::vector<Any>& args) const = 0;

    private:
        std::string m_Name;
        const TypeDescriptor* const m_Parent;
    };



    template <typename Return, typename ...Args>
    class FreeFunction : public Function
    {
    private:
        using FunctionPointer = Return(*)(Args...);

    public:
        FreeFunction(FunctionPointer freeFunctionPointer, const std::string& name) 
            : Function(name, nullptr, Details::Resolve<Return>(), { Details::Resolve<std::remove_cv_t<std::remove_reference_t<Args>>>()... }), m_FreeFunctionPointer(freeFunctionPointer) { }

    private:
        Any InvokeImplementation(Any, std::vector<Any>& args) const override
        {
            return InvokeImplementation(args, std::index_sequence_for<Args...>());
        }

        template <size_t ...Indices>
        Any InvokeImplementation(std::vector<Any>& args, std::index_sequence<Indices...> indexSequence) const
        {
            std::tuple argsTuple = std::make_tuple(args[Indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);
            std::vector<Any> convertedArguments{ (std::get<Indices>(argsTuple) ? AnyReference(*std::get<Indices>(argsTuple))
                : args[Indices].TryConvert<std::remove_cv_t<std::remove_reference_t<Args>>>())... };
            argsTuple = std::make_tuple(convertedArguments[Indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);

            if ((std::get<Indices>(argsTuple) && ...)) // All arguments are valid
            {
                if constexpr (std::is_reference_v<Return>)
                {
                    return AnyReference(m_FreeFunctionPointer(*std::get<Indices>(argsTuple)...));
                }
                else
                {
                    return m_FreeFunctionPointer(*std::get<Indices>(argsTuple)...);
                }
            }
            else
            {
                return Any();
            }
        }

    private:
        FunctionPointer m_FreeFunctionPointer;
    };

    template <typename ...Args>
    class FreeFunction<void, Args...> : public Function
    {
    private:
        using FunctionPointer = void(*)(Args...);

    public:
        Any InvokeImplementation(Any, std::vector<Any>& args) const override
        {
            return InvokeImplementation(args, std::index_sequence_for<Args...>());
        }

        template <size_t ...Indices>
        Any InvokeImplementation(std::vector<Any>& args, std::index_sequence<Indices...> indexSequence) const
        {
            std::tuple argsTuple = std::make_tuple(args[Indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);
            std::vector<Any> convertedArguments{ (std::get<Indices>(argsTuple) ? AnyReference(*std::get<Indices>(argsTuple)) : args[Indices].TryConvert<std::remove_cv_t<std::remove_reference_t<Args>>>())... };
            argsTuple = std::make_tuple(convertedArguments[Indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);

            if ((std::get<Indices>(argsTuple) && ...)) // All arguments are valid.
            {
                m_FreeFunctionPointer(*std::get<Indices>(argsTuple)...);
            }
        }

    private:
        FunctionPointer m_FreeFunctionPointer;
    };

    // With a return type.
    template <typename Class, typename Return, typename ...Args>
    class MemberFunction : public Function
    {
    private:
        using MemberFunctionPointer = Return(Class::*)(Args...);

    public:
        MemberFunction(MemberFunctionPointer memberFunction, const std::string& name)
        : Function(name, Details::Resolve<Class>(), Details::Resolve<Return>(), { Details::Resolve<std::remove_cv_t<std::remove_reference_t<Args>>>()... }), m_MemberFunctionPointer(memberFunction)
        {

        }

    private:
        Any InvokeImplementation(Any object, std::vector<Any>& args) const override
        {
            return InvokeImplementation(object, args, std::make_index_sequence<sizeof...(Args)>());
        }

        template <size_t... Indices>
        Any InvokeImplementation(Any object, std::vector<Any>& args, std::index_sequence<Indices..> indexSequence) const
        {
            std::tuple argsTuple = std::make_tuple(args[Indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);
            std::vector<Any> convertedArguments{ std::get<Indices>(argsTuple) ? AnyReference(*std::get<Indices>(argsTuple)) : args[Indices].TryConvert<std::remove_cv_t<std::remove_reference_t<Args>>>())... };
            argsTuple = std::make_tuple(convertedArguments[Indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);

            if (Class* object = object.TryCast<Class>(); (std::get<Indices>(argsTuple) && ...) && object) // Object is valid and all arguments arew valid.
            {
                if constexpr (std::is_reference_v<Return>)
                {
                    return AnyReference((object->*m_MemberFunctionPointer)(*std::get<Indices>(argsTuple)...));
                }
                else
                {
                    return (object->*m_MemberFunctionPointer)(*std::get<Indices>(argsTuple)...);
                }
            }
            else
            {
                return Any();
            }
        }

    private:
        MemberFunctionPointer m_MemberFunctionPointer;
    };

    // No return type.
    template <typename Class, typename ...Args>
    class MemberFunction<Class, void, Args...> : public Function
    {
    private:
        using MemberFunctionPointer = void(Class::*)(Args...);
        
    public:
        MemberFunction(MemberFunctionPointer memberFunction, const std::string& name)
        : Function(name, Details::Resolve<Class>(), Details::Resolve<void>(), { Details::Resolve<std::remove_cv_t<std::remove_reference_t<Args>>>()... }), m_MemberFunctionPointer(memberFunction)
        {

        }

    private:
        Any InvokeImplementation(Any object, std::vector<Any>& arguments) const override
        {
            return InvokeImplementation(object, args, std::make_index_sequence<sizeof...(Args)>());
        }

        template <size_t ...Indices>
        Any InvokeImplementation(Any object, std::vector<Any>& args, std::index_sequence<Indices...> indexSequence) const
        {
            std::tuple argsTuple = std::make_tuple(args[Indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);
            std::vector<Any> convertedArgs{ (std::get<Indices>(argsTuple) ? AnyReference(*std::get<Indices>(argsTuple)) : args[Indices].TryConvert<std::remove_cv_t<std::remove_reference_t<Args>>>())... };
            argsTuple = std::make_tuple < convertedArgs[Indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);

            if (Class* object = object.TryCast<Class>(); (std::get<Indices>(argsTuple) && ...) && object) // Object is valid and all arguments valid.
            {
                (object->*m_MemberFunctionPointer)(*std::get<Indices>(argsTuple)...);
            }

            return Any();
        }

    private:
        MemberFunctionPointer m_MemberFunctionPointer;
            
    };

    template <typename Class, typename Return, typename ...Args>
    class ConstMemberFunction : public Function
    {
    private:
        using ConstMemberFunctionPointer = Return(Class::*)(Args...) const;

    public:
        ConstMemberFunction(ConstMemberFunctionPointer constMemberFunction, const std::string& name) :
            Function(name, Details::Resolve<Class>(), Details::Resolve<Return>(), { Details::Resolve<std::remove_cv_t<std::remove_reference_t<Args>>>()... }),
            m_ConstMemberFunctionPointer(constMemberFunction) { }

    private:
        Any InvokeImplementation(Any object, std::vector<Any>& args) const override
        {
            return InvokeImplementation(object, args, std::make_index_sequence<sizeof...(Args)>());
        }

        template <size_t ...Indices>
        Any InvokeImplementation(Any object, std::vector<Any>& args, std::index_sequence<Indices...> indexSequence) const
        {
            std::tuple argsTuple = std::make_tuple(args[Indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);
            std::vector<Any> convertedArgs{ (std::get<Indices>(argsTuple) ? AnyReference(*std::get<Indices>(argsTuple)) : args[Indices].TryConvert<std::remove_cv_t<std::remove_reference_t<Args>>>())... };
            argsTuple = std::make_tuple(convertedArgs[Indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);

            if (Class* object = object.TryCast<Class>(); object && (std::get<Indices>(argsTuple) && ...)) // Object is valid and all arguments are valid.
            {
                if constexpr (std::is_void<Return>::value)
                {
                    (object->*m_ConstMemberFunctionPointer)(*std::get<Indices>(argsTuple)...);

                    return Any();
                }
                else
                {
                    if constexpr (std::is_reference_v<Return>)
                    {
                        return AnyReference((object->*m_ConstMemberFunctionPointer)(*std::get<Indices>(argsTuple)...));
                    }
                    else
                    {
                        return (object->*m_ConstMemberFunctionPointer)(*std::get<Indices>(argsTuple)...);
                    }
                }
            }
            else
            {
                return Any();
            }
        }

            
    private:
        ConstMemberFunctionPointer m_ConstMemberFunctionPointer;
    };
}