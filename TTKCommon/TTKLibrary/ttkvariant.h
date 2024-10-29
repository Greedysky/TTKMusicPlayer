#ifndef TTKVARIANT_H
#define TTKVARIANT_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2024 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <typeindex>
#include "ttkmoduleexport.h"

template <size_t arg, size_t... rest>
struct IntegerMax;

template <size_t arg>
struct IntegerMax<arg> : std::integral_constant<size_t, arg> {};

template <size_t arg1, size_t arg2, size_t... rest>
struct IntegerMax<arg1, arg2, rest...> : std::integral_constant<size_t, arg1 >= arg2 ? IntegerMax<arg1, rest...>::value : IntegerMax<arg2, rest...>::value> {};


template <typename... Args>
struct MaxAlign : std::integral_constant<int, IntegerMax<std::alignment_of<Args>::value...>::value> {};


template <typename T, typename... List>
struct Contains : std::true_type {};

template <typename T, typename Head, typename... Rest>
struct Contains<T, Head, Rest...> : std::conditional< std::is_same<T, Head>::value, std::true_type, Contains<T, Rest...>>::type {};

template <typename T>
struct Contains<T> : std::false_type {};


template <typename Type, typename... Types>
struct GetLeftSize;

template <typename Type, typename First, typename... Types>
struct GetLeftSize<Type, First, Types...> : GetLeftSize<Type, Types...> {};

template <typename Type, typename... Types>
struct GetLeftSize<Type, Type, Types...> : std::integral_constant<int, sizeof...(Types)> {};

template <typename Type>
struct GetLeftSize<Type> : std::integral_constant<int, -1> {};


template <typename T, typename... Types>
struct Index : std::integral_constant<int, sizeof...(Types) - GetLeftSize<T, Types...>::value - 1> {};


template <typename... Args>
struct VariantHelper;

template <typename T, typename... Args>
struct VariantHelper<T, Args...>
{
    inline static void Destroy(std::type_index id, void * data)
    {
        if(id == std::type_index(typeid(T)))
            //((T*) (data))->~T();
            reinterpret_cast<T*>(data)->~T();
        else
            VariantHelper<Args...>::Destroy(id, data);
    }

    inline static void move(std::type_index old_t, void *old_v, void *new_v)
    {
        if(old_t == std::type_index(typeid(T)))
            new (new_v) T(std::move(*reinterpret_cast<T*>(old_v)));
        else
            VariantHelper<Args...>::move(old_t, old_v, new_v);
    }

    inline static void copy(std::type_index old_t, const void *old_v, void *new_v)
    {
        if(old_t == std::type_index(typeid(T)))
            new (new_v) T(*reinterpret_cast<const T*>(old_v));
        else
            VariantHelper<Args...>::copy(old_t, old_v, new_v);
    }
};

template <>
struct VariantHelper<>
{
    inline static void Destroy(std::type_index, void *) {}
    inline static void move(std::type_index, void *, void *) {}
    inline static void copy(std::type_index, const void *, void *) {}
};


template <typename... Types>
class TTK_MODULE_EXPORT TTKVariant
{
    enum
    {
        data_size = IntegerMax<sizeof(Types)...>::value,
        align_size = MaxAlign<Types...>::value
    };

    typedef VariantHelper<Types...> Helper_t;
    using data_t = typename std::aligned_storage<data_size, align_size>::type;

public:
    /*!
     * Object constructor.
     */
    TTKVariant()
        : m_type(typeid(void))
    {
    }
    /*!
     * Object constructor.
     */
    TTKVariant(TTKVariant<Types...> &&other)
        : m_type(other.m_type)
    {
        Helper_t::move(other.m_type, &other.m_data, &m_data);
    }
    /*!
     * Object constructor.
     */
    TTKVariant(const TTKVariant<Types...> &other)
        : m_type(other.m_type)
    {
        Helper_t::copy(other.m_type, &other.m_data, &m_data);
    }
    /*!
     * Object destructor.
     */
    ~TTKVariant()
    {
        Helper_t::Destroy(m_type, &m_data);
    }

    TTKVariant& operator=(const TTKVariant &other)
    {
        Helper_t::copy(other.m_type, &other.m_data, &m_data);
        m_type = other.m_type;
        return *this;
    }

    TTKVariant& operator=(TTKVariant &&other)
    {
        Helper_t::move(other.m_type, &other.m_data, &m_data);
        m_type = other.m_type;
        return *this;
    }

    template <class T,
    class = typename std::enable_if<Contains<typename std::remove_reference<T>::type, Types...>::value>::type>
        TTKVariant(T &&value) : m_type(typeid(void))
    {
        Helper_t::Destroy(m_type, &m_data);
        typedef typename std::remove_reference<T>::type U;
        new (&m_data) U(std::forward<T>(value));
        m_type = std::type_index(typeid(T));
    }

    template <typename T>
    bool isSame() const noexcept
    {
        return m_type == std::type_index(typeid(T));
    }

    bool isEmpty() const noexcept
    {
        return m_type == std::type_index(typeid(void));
    }

    std::type_index type() const noexcept
    {
        return m_type;
    }

    template <typename T>
    typename std::decay<T>::type& get()
    {
        using U = typename std::decay<T>::type;
        if(!isSame<U>())
        {
            TTK_ERROR_STREAM(typeid(U).name() << " is not defined. " << "current type is " << m_type.name());
            throw std::bad_cast();
        }
        return *(U*) (&m_data);
    }

    template<typename T>
    int indexOf() const noexcept
    {
        return Index<T, Types...>::value;
    }

    bool operator==(const TTKVariant &other) const noexcept
    {
        return m_type == other.m_type;
    }

    bool operator<(const TTKVariant &other) const noexcept
    {
        return m_type < other.m_type;
    }

private:
    data_t m_data;
    std::type_index m_type;
};


namespace std
{
#if !TTK_HAS_CXX17
// compatiblity for std variant
template <typename... Types>
using variant = TTKVariant<Types...>;
#endif
}

#endif // TTKVARIANT_H
