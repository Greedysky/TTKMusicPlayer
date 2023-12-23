#ifndef TTKGLOBAL_H
#define TTKGLOBAL_H

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

#include <string>

// Normal definition
#undef TTK_STD_CXX
#define TTK_STD_CXX __cplusplus

// VS2013 - 2015
#if defined _MSC_VER && _MSC_VER >= 1800
#  undef TTK_STD_CXX
#  define TTK_STD_CXX 201103L
#endif

// VS2017
#if defined _MSC_VER && _MSC_VER >= 1910
#  undef TTK_STD_CXX
#  define TTK_STD_CXX 201402L
#endif

// VS2019
#if defined _MSC_VER && _MSC_VER >= 1920
#  undef TTK_STD_CXX
#  define TTK_STD_CXX 201703L
#endif

// VS2022
#if defined _MSC_VER && _MSC_VER >= 1930
#  undef TTK_STD_CXX
#  define TTK_STD_CXX 202002L
#endif


#if TTK_STD_CXX >= 202302L  // c++2b
#  define TTK_HAS_CXX11 1
#  define TTK_HAS_CXX14 1
#  define TTK_HAS_CXX17 1
#  define TTK_HAS_CXX20 1
#  define TTK_HAS_CXX23 1
#elif TTK_STD_CXX >= 202002L // c++2a
#  define TTK_HAS_CXX11 1
#  define TTK_HAS_CXX14 1
#  define TTK_HAS_CXX17 1
#  define TTK_HAS_CXX20 1
#  define TTK_HAS_CXX23 0
#elif TTK_STD_CXX >= 201703L // c++1z
#  define TTK_HAS_CXX11 1
#  define TTK_HAS_CXX14 1
#  define TTK_HAS_CXX17 1
#  define TTK_HAS_CXX20 0
#  define TTK_HAS_CXX23 0
#elif TTK_STD_CXX >= 201402L // c++1y
#  define TTK_HAS_CXX11 1
#  define TTK_HAS_CXX14 1
#  define TTK_HAS_CXX17 0
#  define TTK_HAS_CXX20 0
#  define TTK_HAS_CXX23 0
#elif TTK_STD_CXX >= 201103L // c++1x
#  define TTK_HAS_CXX11 1
#  define TTK_HAS_CXX14 0
#  define TTK_HAS_CXX17 0
#  define TTK_HAS_CXX20 0
#  define TTK_HAS_CXX23 0
#else
#  define TTK_HAS_CXX11 0
#  define TTK_HAS_CXX14 0
#  define TTK_HAS_CXX17 0
#  define TTK_HAS_CXX20 0
#  define TTK_HAS_CXX23 0
#endif


#ifdef __GNUC__
#  pragma GCC diagnostic ignored "-Wswitch"
#  pragma GCC diagnostic ignored "-Wparentheses"
#  pragma GCC diagnostic ignored "-Wunused-result"
#  pragma GCC diagnostic ignored "-Wunused-function"
#endif


#if TTK_HAS_CXX11
#  define TTK_CAST
#endif

// cast
#ifdef TTK_CAST
#  define TTKConstCast(x, y) (const_cast<x>(y))
#else
#  define TTKConstCast(x, y) ((x)(y))
#endif

#ifdef TTK_CAST
#  define TTKDynamicCast(x, y) (dynamic_cast<x>(y))
#else
#  define TTKDynamicCast(x, y) ((x)(y))
#endif

#ifdef TTK_CAST
#  define TTKReinterpretCast(x, y) (reinterpret_cast<x>(y))
#else
#  define TTKReinterpretCast(x, y) ((x)(y))
#endif

#ifdef TTK_CAST
#  define TTKStaticCast(x, y) (static_cast<x>(y))
#else
#  define TTKStaticCast(x, y) ((x)(y))
#endif


// marco cat
#define TTK_CAT(a, b) a##b
// marco str cat
#ifndef _MSC_VER
#  define TTK_STR_CAT(...) TTK_PP_OVERLOAD(__TTK_STR_CAT__, __VA_ARGS__)(__VA_ARGS__)
#else
#  define TTK_STR_CAT(...) TTK_PP_CAT(TTK_PP_OVERLOAD(__TTK_STR_CAT__, __VA_ARGS__)(__VA_ARGS__), TTK_PP_EMPTY())
#endif
#define __TTK_STR_CAT__1(a)                a
#define __TTK_STR_CAT__2(a, b)             a b
#define __TTK_STR_CAT__3(a, b, c)          a b c
#define __TTK_STR_CAT__4(a, b, c, d)       a b c d
#define __TTK_STR_CAT__5(a, b, c, d, e)    a b c d e
#define __TTK_STR_CAT__6(a, b, c, d, e, f) a b c d e f


// marco preprocessor overload
#define TTK_PP_OVERLOAD(prefix, ...) TTK_PP_CAT(prefix, TTK_PP_VARIADIC_SIZE(__VA_ARGS__))
#define TTK_PP_CAT(a, b) TTK_CAT(a, b)
#define TTK_PP_EMPTY()
#ifdef _MSC_VER
#  define TTK_PP_VARIADIC_SIZE(...) TTK_PP_CAT(TTK_PP_VARIADIC_SIZE_I(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1,),)
#else
#  define TTK_PP_VARIADIC_SIZE(...) TTK_PP_VARIADIC_SIZE_I(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1,)
#endif
#define TTK_PP_VARIADIC_SIZE_I(e0, e1, e2, e3, e4, e5, e6, e7, size, ...) size


#define TTK_DOT             "."
#define TTK_DOTDOT          ".."
#define TTK_SEPARATOR       "/"
#define TTK_RSEPARATOR      "\\"
#define TTK_PDIR            TTK_STR_CAT(TTK_DOTDOT, TTK_SEPARATOR)

#define TTK_SPLITER         "*|||*"
#define TTK_DEFAULT_STR     "-"
#define TTK_NAN_STR         "NaN"
#define TTK_NULL_STR        "null"


#define HTTP_PREFIX         "http://"
#define HTTPS_PREFIX        "https://"

#define SHL_FILE_SUFFIX     "sh"
#define EXE_FILE_SUFFIX     "exe"
#define COM_FILE_SUFFIX     "com"

#define SHL_FILE            TTK_STR_CAT(TTK_DOT, SHL_FILE_SUFFIX)
#define EXE_FILE            TTK_STR_CAT(TTK_DOT, EXE_FILE_SUFFIX)
#define COM_FILE            TTK_STR_CAT(TTK_DOT, COM_FILE_SUFFIX)


// C style format
using TTKInt8 =             signed char;                /* 8 bit signed */
using TTKUInt8 =            unsigned char;              /* 8 bit unsigned */
using TTKInt16 =            short;                      /* 16 bit signed */
using TTKUInt16 =           unsigned short;             /* 16 bit unsigned */
using TTKInt32 =            int;                        /* 32 bit signed */
using TTKUInt32 =           unsigned int;               /* 32 bit unsigned */
using TTKInt64 =            long long;                  /* 64 bit signed */
using TTKUInt64 =           unsigned long long;         /* 64 bit unsigned */

using TTKReal =             double;                     /* real */
using TTKDouble =           double;                     /* double */
using TTKFloat =            float;                      /* float */
using TTKBool =             bool;                       /* bool */

// C++ style format
using TTKString =           std::string;                /* string */
using TTKWString =          std::wstring;               /* wstring */

#endif // TTKGLOBAL_H
