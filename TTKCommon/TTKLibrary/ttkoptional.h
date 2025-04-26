#ifndef TTKOPTIONAL_H
#define TTKOPTIONAL_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include "ttksmartptr.h"

/*! @brief The class of the optional module.
 * @author Greedysky <greedysky@163.com>
 */
template <typename T>
struct TTKOptional
{
    TTKOptional()
        : m_value(nullptr)
    {

    }

    template <typename... Args>
    static TTKOptional<T> make_optional(Args &&...args)
    {
        TTKOptional<T> res;
        res.emplace(std::forward<Args>(args)...);
        return res;
    }

    TTKOptional(const TTKOptional<T> &other, typename std::enable_if<std::is_copy_constructible<T>::value, int>::type = 0)
        : m_value()
    {
        if(other.m_value)
        {
            m_value = std::make_unique<T>(*other.m_value);
        }
    }

    TTKOptional(const T &other)
        : m_value(std::make_unique<T>(other))
    {

    }

    TTKOptional(TTKOptional &&other)
    {
        m_value = std::move(other.m_value);
    }

    TTKOptional& operator=(TTKOptional &&other)
    {
        m_value = std::move(other.m_value);
        return *this;
    }

    void operator=(const TTKOptional<T> &other)
    {
        if(other.m_value)
        {
            m_value = std::make_unique<T>(*other.m_value);
        }
        else
        {
            m_value.reset();
        }
    }

    bool operator<(const TTKOptional<T> &other) const
    {
        return m_value && other.m_value ? (*m_value < *other.m_value) : (!m_value && other.m_value);
    }

    bool operator<=(const TTKOptional<T> &other) const
    {
        return m_value < other.m_value || m_value == other.m_value;
    }

    bool operator>(const TTKOptional<T> &other) const
    {
        return m_value && other.m_value ? (*m_value > *other.m_value) : (m_value && !other.m_value);
    }

    bool operator>=(const TTKOptional<T> &other) const
    {
        return m_value > other.m_value || m_value == other.m_value;
    }

    template <typename... Args>
    void emplace(Args &&...args)
    {
        m_value = std::make_unique<T>(std::forward<Args>(args)...);
    }

    void reset() { m_value.reset(); }
    bool has_value() const { return m_value.operator bool(); }
    const T& value() const { return *m_value; }

    operator bool() const { return m_value.operator bool(); }
    T* operator->() { return &*m_value; }
    const T* operator->() const { return &*m_value; }

    const T& operator*() const
    {
        return *m_value;
    }

    T& operator*()
    {
        return *m_value;
    }

    bool operator==(const TTKOptional &other) const
    {
        if(!m_value && !other)
        {
            return true;
        }

        return m_value && other ? (*m_value == *other) : false;
    }

    bool operator!=(const TTKOptional &other) const { return !operator==(other); }

private:
    std::unique_ptr<T> m_value;
};


template <typename T>
inline bool operator==(const TTKOptional<T> &v, const T &other)
{
    return v.has_value() ? (*v == other) : false;
}

template <typename T>
inline bool operator==(const T &other, const TTKOptional<T> &v)
{
    return v.has_value() ? (*v == other) : false;
}

template <typename T>
inline bool operator!=(const TTKOptional<T> &v, const T &other)
{
    return !operator==(v, other);
}

template <typename T>
inline bool operator!=(const T &other, const TTKOptional<T> &v)
{
    return !operator==(other, v);
}


// compatiblity for std optional
namespace std
{
#if !TTK_HAS_CXX17
template <typename T>
using optional = TTKOptional<T>;
#endif
}

#endif // TTKOPTIONAL_H
