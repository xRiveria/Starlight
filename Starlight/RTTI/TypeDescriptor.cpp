#include "TypeDescriptor.h"
#include "DataMember.h"
#include "Function.h"
#include "Constructor.h"
#include "Base.h"
#include "Conversion.h"

namespace Reflect
{
    template <typename Type, typename ...Args>
    void TypeDescriptor::AddConstructor()
    {
        Constructor* constructor = new ConstructorImplementation<Type, Args...>();

        m_Constructors.push_back(constructor);
    }

    template <typename Type, typename ...Args>
    void TypeDescriptor::AddConstructor(Type(*Functor)(Args...))
    {
        // Constructor* constructor = new 
    }
}