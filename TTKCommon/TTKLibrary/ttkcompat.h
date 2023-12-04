#ifndef TTKCOMPAT_H
#define TTKCOMPAT_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include "ttkglobal.h"
#include <utility>

#if !TTK_HAS_CXX14
namespace std
{
/// Assign @p __new_val to @p __obj and return its previous value.
template <typename _Tp, typename _Up = _Tp>
inline _Tp exchange(_Tp& __obj, _Up&& __new_val) noexcept
{
    _Tp __old_val = std::move(__obj);
    __obj = std::forward<_Up>(__new_val);
    return __old_val;
}
}
#endif

#endif // TTKCOMPAT_H
