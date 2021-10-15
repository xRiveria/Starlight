#pragma once
#include "TypeFactory.h"

namespace Reflect
{
    class TypeDescriptor;

    /*
        Reflect takes a string which is the mapped name of the reflected type and returns a TypeFactory, which can be used 
        to add meta objects to the type descriptor.
    */

    template <typename Type>
    TypeFactory<Type>& Reflect(const std::string& name)
    {
        return typeFactory<Type>.ReflectType(name);
    }

    /*
        Three ways to get the type descriptor of a type:
        1) With a template type parameter.
        2) With the name of the type (a string).
        3) With an instance of the object. 

        Returns a const pointer to the type descriptor.
    */
    template <typename Type>
    const TypeDescriptor* Resolve()
    {
        return Details::Resolve<Type>();
    }

    inline const TypeDescriptor* Resolve(const std::string& name)
    {
        return Details::Resolve(name); 
    }

    template <typename T, typename = typename std::enable_if<!std::is_convertible<T, std::string>::value>::type>
    const TypeDescriptor* Resolve(T&& object)
    {
        return Details::Resolve(std::forward<T>(object));
    }
}