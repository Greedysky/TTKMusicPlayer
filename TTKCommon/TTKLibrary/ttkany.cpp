#include "ttkany.h"

TTKAny::TTKAny() noexcept
    : m_type(std::type_index(typeid(void)))
{

}

TTKAny::TTKAny(const TTKAny &other) noexcept
    : m_ptr(other.clone())
    , m_type(other.m_type)
{

}

TTKAny::TTKAny(TTKAny &&other) noexcept
    : m_ptr(std::move(other.m_ptr))
    , m_type(std::move(other.m_type))
{

}

bool TTKAny::isNull() const noexcept
{
    return !bool(m_ptr);
}

TTKAny &TTKAny::operator=(const TTKAny &other) noexcept
{
    if(m_ptr == other.m_ptr)
    {
        return *this;
    }

    m_ptr = other.clone();
    m_type = other.m_type;
    return *this;
}
