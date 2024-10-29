#ifndef TTKANY_H
#define TTKANY_H

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

#include <memory>
#include <typeindex>
#include "ttkcompat.h"
#include "ttkmoduleexport.h"

/*! @brief The class of the ttk any module.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKAny
{
public:
    /*!
     * Object constructor.
     */
    TTKAny() noexcept;
    TTKAny(const TTKAny &other) noexcept;
    TTKAny(TTKAny &&other) noexcept;

    template <typename T,
              typename = typename std::enable_if<!std::is_same<typename std::decay<T>::type, TTKAny>::value, T>::type>
    TTKAny(T &&value) noexcept
        : m_ptr(new _Derived<typename std::decay<T>::type>(std::forward<T>(value))),
          m_type(typeid(typename std::decay<T>::type))
    {

    }

    /*!
     * Current container is null or not.
     */
    bool isNull() const noexcept;

    /*!
     * Current container is same or not by type T.
     */
    template <typename T>
    bool isSame() const noexcept
    {
        return m_type == std::type_index(typeid(T));
    }

    /*!
     * Cast value by type T.
     */
    template <typename T>
    T& cast()
    {
        return TTKConstCast(T&, TTKStaticCast(const TTKAny*, this)->cast<T>());
    }

    /*!
     * Cast value by type T.
     */
    template <typename T>
    const T& cast() const
    {
        if(!isSame<T>())
        {
            throw std::bad_cast();
        }

        auto ptr = TTKDynamicCast(_Derived<T>*, m_ptr.get());
        return ptr->m_value;
    }

    /*!
     * Copy object from other.
     */
    TTKAny& operator=(const TTKAny &other) noexcept;

    /*!
     * Swap object from other.
     */
    void swap(TTKAny &other) noexcept
    {
        other = std::exchange(*this, std::move(other));
    }

private:
    struct _Base;
    using _BasePtr = std::unique_ptr<_Base>;

    struct _Base
    {
        virtual ~_Base() noexcept = default;

        virtual _BasePtr clone() const noexcept = 0;
    };

    template <typename T>
    struct _Derived : public _Base
    {
        template <typename U>
        _Derived(U &&value) noexcept
            : m_value(std::forward<U>(value))
        {
        }

        virtual _BasePtr clone() const noexcept override final
        {
            return _BasePtr(new _Derived<T>(m_value));
        }

        T m_value;
    };

    /*!
     * Clone data from this.
     */
    _BasePtr clone() const noexcept
    {
        return m_ptr ? m_ptr->clone() : nullptr;
    }

    _BasePtr m_ptr;
    std::type_index m_type;

};


#ifdef TTK_CAST
#  define TTKAnyCast(x, y) (TTK::any_cast<x>(y))
#else
#  define TTKAnyCast(x, y) ((x)(y))
#endif


/*! @brief The namespace of the any helper.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    /*!
     * Make any helper.
     */
    template <typename T, typename... Args>
    inline TTKAny make_any(Args &&...args)
    {
        return TTKAny(T(std::forward<Args>(args)...));
    }

    template <typename T>
    using remove_cvr = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

    /*!
     * Make any cast helper.
     */
    template <typename T, typename _TYPE = remove_cvr<T>>
    inline _TYPE any_cast(const TTKAny &other)
    {
        return other.isSame<_TYPE>() ? other.cast<_TYPE>() : _TYPE();
    }
    /*!
     * Make any cast helper.
     */
    template <typename T, typename _TYPE = remove_cvr<T>>
    inline _TYPE any_cast(TTKAny &other)
    {
        return other.isSame<_TYPE>() ? other.cast<_TYPE>() : _TYPE();
    }
    /*!
     * Make any cast helper.
     */
    template <typename T, typename _TYPE = remove_cvr<T>>
    inline _TYPE any_cast(TTKAny &&other)
    {
        return other.isSame<_TYPE>() ? other.cast<_TYPE>() : _TYPE();
    }
    /*!
     * Make any cast helper.
     */
    template <typename T, typename _TYPE = remove_cvr<T>>
    inline _TYPE any_cast(const TTKAny *const other) noexcept
    {
        return other->isSame<_TYPE>() ? other->cast<_TYPE>() : _TYPE();
    }
    /*!
     * Make any cast helper.
     */
    template <typename T, typename _TYPE = remove_cvr<T>>
    inline _TYPE any_cast(TTKAny *const other) noexcept
    {
        return other->isSame<_TYPE>() ? other->cast<_TYPE>() : _TYPE();
    }
}


namespace std
{
// Non-member functions [any.nonmembers]
inline void swap(TTKAny &left, TTKAny &right) noexcept
{
    left.swap(right);
}

#if !TTK_HAS_CXX17
// compatiblity for std any
using any = TTKAny;
using namespace TTK;
#endif
}

#endif // TTKANY_H
