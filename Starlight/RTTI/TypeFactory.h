#pragma once
#include <string>
#include <type_traits>
#include "TypeDescriptor.h"

namespace Reflect
{
    // Forward
    template <typename T>
    class TypeFactory;

    // Variable Template (One Type Factory per Type)
    template <typename Type>
    TypeFactory<Type> typeFactory;

    template <typename Type>
    class TypeFactory
    {
    public:
        TypeFactory& ReflectType(const std::string& name)
        {
            TypeDescriptor* typeDescriptor = Details::Resolve<Type>();

            typeDescriptor->m_Name = name;
            Details::GetTypeRegistry()[name] = typeDescriptor;

            return typeFactory<Type>;
        }

        template <typename ...Args>
        TypeFactory& AddConstructor()
        {
            Details::Resolve<Type>()->template AddConstructor<Type, Args...>();

            return typeFactory<Type>;
        }

        template <typename ... Args>
        TypeFactory& AddConstructor(Type(*Functor)(Args...))
        {
            Details::Resolve<Type>()->template AddConstructor<Type, Args...>(Functor);

            return typeFactory<Type>;
        }

        template <typename Base>
        TypeFactory& AddBase()
        {
            static_assert(std::is_base_of<Base, Type>::value); // Base must be a base of Type.

            Details::Resolve<Type>()->template AddBase<Base, Type>();

            return typeFactory<Type>;
        }

        template <typename T, typename U = Type> // Default template type parameter to allow for non-class types.
        TypeFactory& AddDataMember(T U::* dataMemberPointer, const std::string& name)
        {
            Details::Resolve<Type>()->AddDataMember(dataMemberPointer, name);

            return typeFactory<Type>;
        }

        template <auto Setter, auto Getter>
        TypeFactory& AddDataMember(const std::string& name)
        {
            Details::Resolve<Type>()->template AddDataMember<Setter, Getter, Type>(name);

            return typeFactory<Type>;
        }

        template <typename Ret, typename ...Args>
        TypeFactory& AddMemberFunction(Ret(*Functor)(Args...), const std::string& name)
        {
            Details::Resolve<Type>()->AddMemberFunction(Functor, name);

            return typeFactory<Type>;
        }

        template <typename Ret, typename ...Args, typename U = Type>
        TypeFactory& AddMemberFunction(Ret(U::* Functor)(Args...), const std::string& name)
        {
            Details::Resolve<Type>()->AddMemberFunction(Functor, name);

            return typeFactory<Type>;
        }

        template <typename Ret, typename ...Args, typename U = Type>
        TypeFactory& AddMemberFunction(Ret(U::* constFunctor)(Args...) const, const std::string& name)
        {
            Details::Resolve<Type>()->AddMemberFunction(constFunctor, name);

            return typeFactory<Type>;
        }

        template <typename To>
        TypeFactory& AddConversion()
        {
            static_assert(std::is_convertible_v<Type, To>); // A conversion from Type -> To must exist.

            Details::Resolve<Type>()->template AddConversion<Type, To>();

            return typeFactory<Type>;
        }
    };
}