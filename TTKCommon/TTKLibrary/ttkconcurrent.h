#ifndef TTKCONCURRENT_H
#define TTKCONCURRENT_H

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

#include "ttkqtglobal.h"

#if TTK_QT_VERSION_CHECK(5,0,0)
#  include <QtConcurrent/QtConcurrent>
#else
#  include <QtConcurrentRun>
#endif


#define TTKConcurrent_1(data) \
    const auto TTK_CAT(ext_status_, __LINE__) = QtConcurrent::run([&]() data );

#ifndef Q_CC_MSVC
#  define TTKConcurrent(...) TTK_PP_OVERLOAD(TTKConcurrent_, __VA_ARGS__)(__VA_ARGS__)
#else
#  define TTKConcurrent(...) TTK_PP_CAT(TTK_PP_OVERLOAD(TTKConcurrent_, __VA_ARGS__)(__VA_ARGS__), TTK_PP_EMPTY())
#endif

#endif // TTKCONCURRENT_H
