#pragma once
#include "TypeDescriptor.h"

namespace Reflect
{
    class Base
    {
    public:
        const TypeDescriptor* GetType() const { return m_Type; }
        virtual void* Cast(void* object) = 0;

    protected:
        Base(const TypeDescriptor* type, const TypeDescriptor* parent) : m_Parent(parent), m_Type(type) {}

    private:
        const TypeDescriptor* m_Parent;
        const TypeDescriptor* m_Type;
    };

    template <typename B, typename D>
    class BaseImplementation : public Base
    {
    public:
        BaseImplementation() : Base(Details::Resolve<B>(), Details::Resolve<D>()) {}

        void* Cast(void* object) override
        {
            return static_cast<B*>(object);
        }
    };
}