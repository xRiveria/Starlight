#ifndef TYPE_FACTORY_H
#define TYPE_FACTORY_H

#include <string>
#include "TypeDescriptor.hpp"

namespace RTTI
{
	// Forward Declaration
	template <typename Type>
	class TypeFactory;

	// Variable Template (One Type Factory Instantiation per Type)
	template <typename Type>
	TypeFactory<Type> typeFactory;

	template <typename Type>
	class TypeFactory
	{
	public:
		TypeFactory& ReflectType(const std::string &name)
		{
			// Create a new Type Descriptor object for our newly serialized type.
			TypeDescriptor* typeDescriptor = Details::Resolve<Type>();

			// Sets its name internally.
			typeDescriptor->m_Name = name;

			// Registers the new type in our registry.
			Details::GetTypeRegistry()[name] = typeDescriptor;

			// Returns the type factory for this object.
			return typeFactory<Type>;
		}

		template <typename... Args>
		TypeFactory& AddConstructor()
		{
			Details::Resolve<Type>()->template AddConstructor<Type, Args...>();

			return typeFactory<Type>;
		}

		template <typename... Args>
		TypeFactory& AddConstructor(Type (*ctorFun)(Args...))
		{
			Details::Resolve<Type>()->template AddConstructor<Type, Args...>(ctorFun);

			return typeFactory<Type>;
		}

		template <typename Base>
		TypeFactory& AddBase()
		{
			static_assert(std::is_base_of<Base, Type>::value);  // Base must be a base of Type.

			Details::Resolve<Type>()->template AddBase<Base, Type>();

			return typeFactory<Type>;
		}

		template <typename T, typename U = Type>  // Default template type parameter to allow for non-class types.
		TypeFactory& AddDataMember(T U::*dataMemPtr, const std::string &name)
		{
			Details::Resolve<Type>()->AddDataMember(dataMemPtr, name);

			return typeFactory<Type>;
		}

		template <auto Setter, auto Getter>
		TypeFactory& AddDataMember(const std::string &name)
		{
			Details::Resolve<Type>()->template AddDataMember<Setter, Getter, Type>(name);

			return typeFactory<Type>;
		}

		template <typename Return, typename... Args>
		TypeFactory& AddMemberFunction(Return(*freeFun)(Args...), const std::string &name)
		{
			Details::Resolve<Type>()->AddMemberFunction(freeFun, name);

			return typeFactory<Type>;
		}

		template <typename Return, typename... Args, typename U = Type>
		TypeFactory& AddMemberFunction(Return(U::*memFun)(Args...), const std::string &name)
		{
			Details::Resolve<Type>()->AddMemberFunction(memFun, name);

			return typeFactory<Type>;
		}

		template <typename Return, typename... Args, typename U = Type>
		TypeFactory& AddMemberFunction(Return(U::*constMemFun)(Args...) const, const std::string &name)
		{
			Details::Resolve<Type>()->AddMemberFunction(constMemFun, name);

			return typeFactory<Type>;
		}

		template <typename To>
		TypeFactory& AddConversion()
		{
			static_assert(std::is_convertible_v<Type, To>);  // A conversion from Type -> To must exist.

			Details::Resolve<Type>()->template AddConversion<Type, To>();

			return typeFactory<Type>;
		}
	};
} 

#endif
