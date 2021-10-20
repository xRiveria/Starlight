#ifndef REFLECT_H
#define REFLECT_H

#include "TypeFactory.hpp"

namespace RTTI
{
	class TypeDescriptor;

	// Reflect takes a string which is the mapped name of the reflected type and returns a TypeFactory, which can be used to add meta objects to the type descriptor.
	template <typename Type>
	TypeFactory<Type>& Reflect(const std::string &name)
	{
		return typeFactory<Type>.ReflectType(name);
	}

	/* There are three ways to obtain a type descriptor:
	   - 1) With a template type parameter.
	   - 2) With the name of a type.
	   - 3) With an instance of the object.
       
	   A constant pointer to the type descriptor will be returned.
	*/

	template <typename Type>
	const TypeDescriptor* GetType()
	{
		return Details::Resolve<Type>();
	}

	inline const TypeDescriptor* GetType(const std::string& name)
	{
		return Details::Resolve(name);
	}

	template <typename T, typename = typename std::enable_if<!std::is_convertible<T, std::string>::value>::type>
	const TypeDescriptor* GetType(T&& object)
	{
		return Details::Resolve(std::forward<T>(object));
	}
}

#endif