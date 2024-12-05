#ifndef TTKDEFER_H
#define TTKDEFER_H

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

#include <type_traits>
#include "ttkmoduleexport.h"

/*! @brief The class of the ttk defer module.
 * @author Greedysky <greedysky@163.com>
 */
template <typename Function>
class _ScopeGuard
{
public:
    /*!
     * Object constructor.
     */
    _ScopeGuard(Function &&f)
        : m_fun(std::forward<Function>(f))
    {

    }

    _ScopeGuard(_ScopeGuard &&rhs)
        : m_fun(std::move(rhs.m_fun))
    {

    }

    /*!
     * Object destructor.
     */
    ~_ScopeGuard()
    {
        if(m_fun)
        {
            m_fun();
        }
    }

    TTK_DISABLE_INIT_COPY(_ScopeGuard)

private:
    Function m_fun;

};


/*! @brief The namespace of the defer helper.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
enum class ScopeGuardOnExit { };
}


template <typename Function>
inline _ScopeGuard<Function> operator+(TTK::ScopeGuardOnExit, Function &&fn)
{
    return _ScopeGuard<Function>(std::forward<Function>(fn));
}

// Helper macro
#define defer const auto TTK_CAT(ext_exitBlock_, __LINE__) = TTK::ScopeGuardOnExit() + [&]()

#endif // TTKDEFER_H
