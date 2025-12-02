#ifndef TTKSMARTPTR_H
#define TTKSMARTPTR_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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
#include "ttkcompat.h"

namespace std
{
// compatiblity for std make_unique
#if !TTK_HAS_CXX14
template <typename _Tp>
struct _MakeUnique
{
    typedef unique_ptr<_Tp> __single_object;
};

template <typename _Tp>
struct _MakeUnique<_Tp[]>
{
    typedef unique_ptr<_Tp[]> __array;
};

template <typename _Tp, size_t _Bound>
struct _MakeUnique<_Tp[_Bound]>
{
    struct __invalid_type { };
};

/// std::make_unique for single objects
template <typename _Tp, typename... _Args>
inline typename _MakeUnique<_Tp>::__single_object make_unique(_Args &&...__args)
{
    return unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...));
}

/// std::make_unique for arrays of unknown bound
template <typename _Tp>
inline typename _MakeUnique<_Tp>::__array make_unique(size_t __num)
{
    return unique_ptr<_Tp>(new remove_extent_t<_Tp>[__num]());
}

/// Disable std::make_unique for arrays of known bound
template <typename _Tp, typename... _Args>
inline typename _MakeUnique<_Tp>::__invalid_type make_unique(_Args &&...) = delete;
#endif
}

#if !TTK_HAS_CXX11
class _SharedCount
{
public:
    _SharedCount() noexcept 
        : m_count(1)
    {
    
    }

    std::atomic<int> m_count;
};

template <typename T>
class TTKSharedPtr
{
public:
    TTKSharedPtr() noexcept
        : m_ptr(nullptr),
          m_ref_count(new _SharedCount)
    {
    
    }

    TTKSharedPtr(T* ptr) noexcept
        : m_ptr(ptr),
          m_ref_count(new _SharedCount)
    {
    
    }

    ~TTKSharedPtr() noexcept
    {
        clean();
    }

    template <typename U>
    friend class TTKSharedPtr;

    template <typename U>
    TTKSharedPtr(const TTKSharedPtr<U> &p, T* ptr) noexcept
    {
        m_ptr = ptr;
        m_ref_count = p.m_ref_count;
        m_ref_count->m_count++;
    }

    TTKSharedPtr(const TTKSharedPtr &p) noexcept
    {
        m_ptr = p.m_ptr;
        m_ref_count = p.m_ref_count;
        m_ref_count->m_count++;
    }

    TTKSharedPtr& operator=(const TTKSharedPtr &p) noexcept
    {
        clean();
        m_ptr = p.m_ptr;
        m_ref_count = p.m_ref_count;
        m_ref_count->m_count++;
        return *this;
    }

    TTKSharedPtr(TTKSharedPtr &&p) noexcept
    {
        m_ptr = p.m_ptr;
        m_ref_count = p.m_ref_count;
        p.m_ptr = nullptr;
        p.m_ref_count = nullptr;
    }

    TTKSharedPtr& operator=(TTKSharedPtr &&p) noexcept
    {
        clean();
        m_ptr = p.m_ptr;
        m_ref_count = p.m_ref_count;
        p.m_ptr = nullptr;
        p.m_ref_count = nullptr;
        return *this;
    }

    inline int use_count() noexcept { return m_ref_count->m_count; }
    inline T* get() const noexcept { return m_ptr; }
    inline T* operator->() const noexcept { return m_ptr; }
    inline T& operator*() const noexcept { return *m_ptr; }
    inline operator bool() const noexcept { return m_ptr; }

private:
    void clean() noexcept
    {
        if(m_ref_count)
        {
            m_ref_count->m_count--;
            if(m_ref_count->m_count == 0)
            {
                delete m_ptr;
                delete m_ref_count;
            }
        }
    }

    T* m_ptr;
    _SharedCount* m_ref_count;
};

namespace TTK
{
template <typename T, typename... Args>
inline TTKSharedPtr<T> make_shared(Args &&...args)
{
    return TTKSharedPtr<T>(std::forward<Args>(args)...);
}

template <typename T, typename U>
inline TTKSharedPtr<T> static_pointer_cast(const TTKSharedPtr<U> &p) noexcept
{
    T* ptr = static_cast<T*>(p.get());
    return TTKSharedPtr<T>(p, ptr);
}

template <typename T, typename U>
inline TTKSharedPtr<T> const_pointer_cast(const TTKSharedPtr<U> &p) noexcept
{
    T* ptr = const_cast<T*>(p.get());
    return TTKSharedPtr<T>(p, ptr);
}

template <typename T, typename U>
inline TTKSharedPtr<T> dynamic_pointer_cast(const TTKSharedPtr<U> &p) noexcept
{
    T* ptr = dynamic_cast<T*>(p.get());
    return ptr == nullptr ? TTKSharedPtr<T>() : TTKSharedPtr<T>(p, ptr);
}

template <typename T, typename U>
inline TTKSharedPtr<T> reinterpret_pointer_cast(const TTKSharedPtr<U> &p) noexcept
{
    T* ptr = reinterpret_cast<T*>(p.get());
    return TTKSharedPtr<T>(p, ptr);
}
}

namespace std
{
// compatiblity for std shared_ptr
template <typename T>
using shared_ptr = TTKSharedPtr<T>;
using namespace TTK;
}
#endif

namespace TTK
{
template <typename T>
inline std::shared_ptr<T> makeCopy(const T* &source)
{
    return std::make_shared<T>(*source);
}

template <typename T>
inline std::shared_ptr<T> makeCopy(const std::shared_ptr<T> &source)
{
    return std::make_shared<T>(*source);
}
}

#endif // TTKSMARTPTR_H
