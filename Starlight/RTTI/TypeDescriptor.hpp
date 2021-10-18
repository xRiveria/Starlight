#ifndef TYPE_DESCRIPTOR_H
#define TYPE_DESCRIPTOR_H

#include <string>
#include <vector>
#include <map>

namespace Reflect
{
	// fwd declarations
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
		TypeDescriptor *Resolve();

		template <typename Type>
		TypeDescriptor *Resolve(Type&&);
	}	

	class TypeDescriptor
	{
		template <typename> friend class TypeFactory;

		template <typename Type> friend TypeDescriptor *Details::Resolve();
		template <typename Type> friend TypeDescriptor *Details::Resolve(Type&&);

	public:
		template <typename Type, typename... Args>
		void AddConstructor();

		template <typename Type, typename... Args>
		void AddConstructor(Type (*)(Args...));

		template <typename B, typename T>
		void AddBase();

		template <typename C, typename T>
		void AddDataMember(T C::*dataMemPtr, const std::string &name);

		template <auto Setter, auto Getter, typename Type>
		void AddDataMember(const std::string &name);

		template <typename Ret, typename... Args>
		void AddMemberFunction(Ret freeFun(Args...), const std::string &name);

		template <typename C, typename Ret, typename... Args>
		void AddMemberFunction(Ret(C::*memFun)(Args...), const std::string &name);

		template <typename C, typename Ret, typename... Args>
		void AddMemberFunction(Ret(C::*memFun)(Args...) const, const std::string &name);

		template <typename From, typename To>
		void AddConversion();

		std::string const &GetName() const;

		//std::size_t GetSize() const;

		std::vector<Constructor*> GetConstructors() const;

		template <typename... Args>
		const Constructor *GetConstructor() const;

		std::vector<Base*> GetBases() const;

		template <typename B>
		Base *GetBase() const;

		std::vector<DataMember*> GetDataMembers() const;

		DataMember *GetDataMember(const std::string &name) const;

		std::vector<Function*> GetMemberFunctions() const;

		const Function *GetMemberFunction(const std::string &name) const;

		std::vector<Conversion*> GetConversions() const;

		template <typename To>
		Conversion *GetConversion() const;

	private:
		std::string mName;
		//std::size_t mSize;

		std::vector<Base*> mBases;
		std::vector<Conversion*> mConversions;
		std::vector<Constructor*> mConstructors;
		std::vector<DataMember*> mDataMembers;
		std::vector<Function*> mMemberFunctions;

		bool mIsPointer;
		bool mIsReference;
		bool mIsFunction;
		bool mIsDataMember;
		bool mIsMemberFunction;
	};

	namespace Details
	{
		template <typename T>
		struct remove_all_ref_ptr
		{
			using type = T;
		};

		template <typename T>
		struct remove_all_ref_ptr<T*> : remove_all_ref_ptr<T>
		{
			//using type = typename remove_all_pointers<T>::type;
		};

		template <typename T>
		struct remove_all_ref_ptr<T* const> : remove_all_ref_ptr<T> {};

		template <typename T>
		struct remove_all_ref_ptr<T&> : remove_all_ref_ptr<T> {};

		template <typename T>
		struct remove_all_ref_ptr<T&&> : remove_all_ref_ptr<T> {};

		template <typename T>
		using remove_all_ref_ptr_t = typename remove_all_ref_ptr<T>::type;

		template <typename T>
		using RawType = typename std::remove_cv<remove_all_ref_ptr_t<T>>::type;

		template <typename T>
		static TypeDescriptor &GetTypeDescriptor()
		{
			static TypeDescriptor typeDescriptor;  // single instance of type descriptor per reflected type

			return typeDescriptor;
		}

		template <typename Type>
		TypeDescriptor *&GetTypeDescriptorPtr()
		{
			static TypeDescriptor *typeDescriptorPtr = nullptr;

			return typeDescriptorPtr;
		}

		inline auto &GetTypeRegistry()
		{
			static std::map<std::string, TypeDescriptor*> typeRegistry;

			return typeRegistry;
		}

		template <typename Type>
		TypeDescriptor *Resolve()
		{
			if (!GetTypeDescriptorPtr<RawType<Type>>())
			{
				GetTypeDescriptorPtr<RawType<Type>>() = &GetTypeDescriptor<RawType<Type>>();  // create a type descriptor if not present

				//if constexpr (std::is_same<RawType<Type>, void>::value)
				//	typeDescriptor<RawType<Type>>.mSize = 0;
				//else
				//	typeDescriptor<RawType<Type>>.mSize = sizeof(RawType<Type>);
			}

			return GetTypeDescriptorPtr<RawType<Type>>();
		}

		inline TypeDescriptor *Resolve(const std::string &name)
		{
			if (auto it = GetTypeRegistry().find(name); it != GetTypeRegistry().end())
				return it->second;

			return nullptr;
		}

		template <typename Type>
		TypeDescriptor *Resolve(Type &&object)
		{
			if (!GetTypeDescriptorPtr<RawType<Type>>())
			{
				GetTypeDescriptorPtr<RawType<Type>>() = &GetTypeDescriptor<RawType<Type>>();   // create a type descriptor if not present

				//if constexpr (std::is_same<RawType<Type>, void>::value)
				//	typeDescriptor<RawType<Type>>.mSize = 0;
				//else
				//	typeDescriptor<RawType<Type>>.mSize = sizeof(RawType<Type>);
			}

			return GetTypeDescriptorPtr<RawType<Type>>();
		}

	}  // namespace Details

}  // namespace Reflect

#include "DataMember.hpp"
#include "Function.hpp"
#include "Constructor.hpp"
#include "Base.hpp"
#include "Conversion.hpp"

#include "TypeDescriptor.inl"

#endif // TYPE_DESCRIPTOR_H