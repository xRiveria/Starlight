#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H

#include <vector>
#include <tuple>
#include "TypeDescriptor.hpp"
#include "Any.hpp"
#include "Conversion.hpp"
#include "Base.hpp"

namespace RTTI
{
	inline bool CanCastOrConvert(const TypeDescriptor* from, const TypeDescriptor* to)
	{
		if (from == to) // If our types are the same...
		{
			return true;
		}

		for (auto* base : from->GetBases()) // Check if the base type is the same...
		{
			if (base->GetType() == to)
			{
				return true;
			}
		}

		for (auto* conversion : from->GetConversions()) // Else, check if a conversione exists between the two types...
		{
			if (conversion->GetToType() == to)
			{
				return true;
			}
		}

		return false; // Gives up...
	}

	class Constructor
	{
	public:
		Any NewInstance(std::vector<Any>& args)
		{
			if (args.size() == m_ParameterTypes.size())
			{
				return NewInstanceImplementation(args);
			}

			return Any();
		}

		template <typename... Args>
		Any NewInstance(Args&&... args) const
		{
			if (sizeof...(Args) == m_ParameterTypes.size())
			{
				auto argsAny = std::vector<Any>({ Any(std::forward<Args>(args))... });
				return NewInstanceImplementation(argsAny);
			}

			return Any();
		}

		const TypeDescriptor* GetParent() const
		{
			return m_Parent;
		}

		const TypeDescriptor* GetParamType(size_t index) const
		{
			return m_ParameterTypes[index];
		}

		size_t GetParameterCount() const
		{
			return m_ParameterTypes.size();
		}

		template <typename... Args, size_t ...indices>
		bool CanConstruct(std::index_sequence<indices...> indexSequence = std::index_sequence_for<Args...>()) const
		{
			return GetParameterCount() == sizeof...(Args) && ((RTTI::CanCastOrConvert(Details::Resolve<Args>(), GetParamType(indices))) && ...);
		}

	protected:
		Constructor(TypeDescriptor* parent, const std::vector<const TypeDescriptor*>& parameterTypes) : m_Parent(parent), m_ParameterTypes(parameterTypes) {}

	private:
		virtual Any NewInstanceImplementation(std::vector<Any>& args) const = 0;

	private:
		TypeDescriptor* m_Parent;
		std::vector<const TypeDescriptor*> m_ParameterTypes;
	};

	template <typename Type, typename ...Args>
	class ConstructorImplementation : public Constructor
	{
	public:
		ConstructorImplementation() : Constructor(Details::Resolve<Details::RawType<Type>>(), { Details::Resolve<Details::RawType<Args>>()... }) {}

	private:
		Any NewInstanceImplementation(std::vector<Any>& args) const override
		{
			return NewInstanceImplementation(args, std::make_index_sequence<sizeof...(Args)>());
		}

		template <size_t... indices>
		Any NewInstanceImplementation(std::vector<Any>& args, std::index_sequence<indices...> indexSequence) const
		{
			std::tuple argsTuple = std::make_tuple(args[indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);
			std::vector<Any> convertedArgs{ (std::get<indices>(argsTuple) ? AnyRef(*std::get<indices>(argsTuple)) : args[indices].TryConvert<std::remove_cv_t<std::remove_reference_t<Args>>>())... };
			argsTuple = std::make_tuple(convertedArgs[indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);

			if ((std::get<indices>(argsTuple) && ...))
			{
				return Type(*std::get<indices>(argsTuple)...);
			}

			return Any();
		}
	};

	template <typename Type, typename ...Args>
	class FreeFunctionConstructor : public Constructor
	{
	private:
		typedef Type(*FreeFunction)(Args...);

	public:
		FreeFunctionConstructor(FreeFunction freeFunction) : Constructor(Details::Resolve<Details::RawType<Type>>(), { Details::Resolve<Details::RawType<Args>>()... }), m_FreeFunction(freeFunction) {}
	
	private:
		Any NewInstanceImplementation(std::vector<Any>& args) const override
		{
			return NewInstanceImpl(args, std::make_index_sequence<sizeof...(Args)>());
		}

		template <size_t ...indices>
		Any NewInstanceImplementation(std::vector<Any>& args, std::index_sequence<indices...> indexSequence) const
		{
			std::tuple argsTuple = std::make_tuple(args[indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);
			std::vector<Any> convertedArgs{ (std::get<indices>(argsTuple) ? AnyRef(*std::get<indices>(argsTuple)) : args[indices].TryConvert<std::remove_cv_t<std::remove_reference_t<Args>>>())... };
			argsTuple = std::make_tuple(convertedArgs[indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);

			if ((std::get<indices>(argsTuple) && ...))
			{
				return m_FreeFunction(*std::get<indices>(argsTuple)...);
			}

			return Any();
		}
		
	private:
		FreeFunction m_FreeFunction;
	};
}
#endif
