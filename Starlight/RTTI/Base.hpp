#ifndef BASE_H
#define BASE_H
#include "TypeDescriptor.hpp"

namespace RTTI
{
	class Base
	{
	public:
		const TypeDescriptor* GetType() const { return m_BaseType; }

		virtual void* Cast(void* object) = 0; 

	protected:
		Base(TypeDescriptor* const type, const TypeDescriptor* parent) : m_ParentType(parent), m_BaseType(type) {}

	private:
		const TypeDescriptor* m_ParentType;
		const TypeDescriptor* m_BaseType;
	};

	template <typename Type, typename Base>
	class BaseImplementation : public Base
	{
	public:
		BaseImplementation() : Base(Details::Resolve<Base>(), Details::Resolve<Type>()) {}

		// Cast any incoming object into our base type.
		void* Cast(void* object) override
		{
			return static_cast<Base*>(object);
		}
	};
}

#endif

