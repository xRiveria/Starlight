#ifndef DATA_MEMBER_H
#define DATA_MEMBER_H

#include <string>
#include "TypeDescriptor.hpp"
#include "Any.hpp"

namespace RTTI
{
	class DataMember
	{
	public:
		std::string GetName() const { return m_Name; }
		const TypeDescriptor* GetParent() const { return m_DataClass; }
		const TypeDescriptor* GetType() const { return m_DataType; }

		virtual void Set(AnyRef objectReference, const Any value) = 0;
		virtual Any Get(Any object) = 0;

	protected:
		DataMember(const std::string &name, const TypeDescriptor *type, const TypeDescriptor *parent)
			     : m_Name(name), m_DataType(type), m_DataClass(parent) { }

	private:
		std::string m_Name;                 
		const TypeDescriptor* m_DataType;    // Type of the data member.
		const TypeDescriptor* m_DataClass;   // Type of the data member's class
	};

	template <typename Class, typename Type>
	class PtrDataMember : public DataMember
	{
	public:
		PtrDataMember(Type Class::*dataMemberPtr, const std::string name)
			: DataMember(name, Details::Resolve<Type>(), Details::Resolve<Class>()), mDataMemberPtr(dataMemberPtr) {}

		void Set(AnyRef objectRef, const Any value) override
		{
			SetImpl(objectRef, value, std::is_const<Type>());  // use tag dispatch
		}

		Any Get(Any object) override
		{
			Class *obj = object.TryCast<Class>();

			if (!obj)
			{
				throw BadCastException(Details::Resolve<Class>()->GetName(), object.GetType()->GetName());
			}

			return obj->*mDataMemberPtr;
		}

	private:
		Type Class::*mDataMemberPtr;

	private:
		// Use tag dispatch.
		void SetImpl(Any object, const Any value, std::false_type)
		{
			Class *obj = object.TryCast<Class>();  // pointers to members of base class can be used with derived class

			if (!obj)
				throw BadCastException(Details::Resolve<Class>()->GetName(), object.GetType()->GetName(), "object:");

			Type const *casted = nullptr;
			Any val;
			if (casted = value.TryCast<Type>(); !casted)
			{
				val = value.TryConvert<Type>();
				casted = val.TryCast<Type>();
			}

			if (!casted)
				throw BadCastException(Details::Resolve<Type>()->GetName(), value.GetType()->GetName(), "value:");

			obj->*mDataMemberPtr = *casted;
		}

		void SetImpl(Any object, const Any value, std::true_type)
		{
			//static_assert(false, "can't set const data member");
		}
	};

	// helper meta function to get info about functions passed as auto non type params (C++17)
	template <typename>
	struct FunctionHelper;

	template <typename Ret, typename... Args>
	struct FunctionHelper<Ret(Args...)>
	{
		using ReturnType = Ret;
		using ParamsTypes = std::tuple<Args...>;
	};

	template <typename Class, typename Ret, typename... Args>
	/*constexpr*/ FunctionHelper<Ret(/*Class, */Args...)> ToFunctionHelper(Ret(Class::*)(Args...));

	template <typename Class, typename Ret, typename... Args>
	/*constexpr*/ FunctionHelper<Ret(/*Class, */Args...)> ToFunctionHelper(Ret(Class::*)(Args...) const);

	template <typename Ret, typename... Args>
	/*constexpr*/ FunctionHelper<Ret(Args...)> ToFunctionHelper(Ret(*)(Args...));

	template <auto Setter, auto Getter, typename Class>
	class SetGetDataMember : public DataMember
	{
	private:
		using MemberType = Details::RawType<typename decltype(ToFunctionHelper(Getter))::ReturnType>;

	public:
		SetGetDataMember(const std::string name) : DataMember(name, Details::Resolve<MemberType>(), Details::Resolve<Class>()) {}

		void Set(AnyRef objectRef, const Any value) override
		{
			Any a = objectRef;
			Class *obj = a.TryCast<Class>();

			if (!obj)
			{
				throw BadCastException(Details::Resolve<Class>()->GetName(), Any(objectRef).GetType()->GetName(), "object:");
			}

			MemberType const *casted = nullptr;
			Any val;
			if (casted = value.TryCast<MemberType>(); !casted)
			{
				val = value.TryConvert<MemberType>();
				casted = val.TryCast<MemberType>();
			}

			if (!casted)
			{
				throw BadCastException(Details::Resolve<MemberType>()->GetName(), value.GetType()->GetName(), "value:");
			}

			if constexpr (std::is_member_function_pointer_v<decltype(Setter)>)
			{
				(obj->*Setter)(*casted);
			}
			else
			{
				static_assert(std::is_function_v<std::remove_pointer_t<decltype(Setter)>>);

				Setter(*obj, *casted);
			}
		}

		Any Get(Any object) override
		{
			Class *obj = object.TryCast<Class>();

			if (!obj)
			{
				throw BadCastException(Details::Resolve<Class>()->GetName(), object.GetType()->GetName());
			}

			if constexpr (std::is_member_function_pointer_v<decltype(Setter)>)
			{
				return (obj->*Getter)();
			}
			else
			{
				static_assert(std::is_function_v<std::remove_pointer_t<decltype(Getter)>>);

				return Getter(*obj);
			}
		}
	};
}

#endif
