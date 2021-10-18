#ifndef TYPE_FACTORY_H
#define TYPE_FACTORY_H

#include <string>
#include "TypeDescriptor.hpp"

namespace Reflect
{

	// fwd declaration
	template <typename Type>
	class TypeFactory;

	// variable template (one type factory per type)
	template <typename Type>
	TypeFactory<Type> typeFactory;

	template <typename Type>
	class TypeFactory
	{
	public:
		TypeFactory &ReflectType(const std::string &name)
		{
			TypeDescriptor *typeDescriptor = Details::Resolve<Type>();

			typeDescriptor->mName = name;
			Details::GetTypeRegistry()[name] = typeDescriptor;

			return typeFactory<Type>;
		}

		template <typename... Args>
		TypeFactory &AddConstructor()
		{
			Details::Resolve<Type>()->template AddConstructor<Type, Args...>();

			return typeFactory<Type>;
		}

		template <typename... Args>
		TypeFactory &AddConstructor(Type (*ctorFun)(Args...))
		{
			Details::Resolve<Type>()->template AddConstructor<Type, Args...>(ctorFun);

			return typeFactory<Type>;
		}

		template <typename Base>
		TypeFactory &AddBase()
		{
			static_assert(std::is_base_of<Base, Type>::value);  // Base must be a base of Type

			Details::Resolve<Type>()->template AddBase<Base, Type>();

			return typeFactory<Type>;
		}

		template <typename T, typename U = Type>  // default template type param to allow for non class types
		TypeFactory &AddDataMember(T U::*dataMemPtr, const std::string &name)
		{
			Details::Resolve<Type>()->AddDataMember(dataMemPtr, name);

			return typeFactory<Type>;
		}

		template <auto Setter, auto Getter>
		TypeFactory &AddDataMember(const std::string &name)
		{
			Details::Resolve<Type>()->template AddDataMember<Setter, Getter, Type>(name);

			return typeFactory<Type>;
		}

		template <typename Ret, typename... Args>
		TypeFactory &AddMemberFunction(Ret(*freeFun)(Args...), const std::string &name)
		{
			Details::Resolve<Type>()->AddMemberFunction(freeFun, name);

			return typeFactory<Type>;
		}

		template <typename Ret, typename... Args, typename U = Type>
		TypeFactory &AddMemberFunction(Ret(U::*memFun)(Args...), const std::string &name)
		{
			Details::Resolve<Type>()->AddMemberFunction(memFun, name);

			return typeFactory<Type>;
		}

		template <typename Ret, typename... Args, typename U = Type>
		TypeFactory &AddMemberFunction(Ret(U::*constMemFun)(Args...) const, const std::string &name)
		{
			Details::Resolve<Type>()->AddMemberFunction(constMemFun, name);

			return typeFactory<Type>;
		}

		//template <typename FuncType, FuncType Func>
		//template <auto Func>
		//TypeFactory &AddMemberFunction(const std::string &name)
		//{
		//	Details::Resolve<Type>()->template AddMemberFunction<FunctType, Func>(name);
		//	Details::Resolve<Type>()->template AddMemberFunction<Func>(name);

		//	return typeFactory<Type>;
		//}

		template <typename To>
		TypeFactory &AddConversion()
		{
			static_assert(std::is_convertible_v<Type, To>);  // a conversion Type -> To must exist

			Details::Resolve<Type>()->template AddConversion<Type, To>();

			return typeFactory<Type>;
		}
	};

}  // namespace Reflect

#endif // TYPE_FACTORY_H
