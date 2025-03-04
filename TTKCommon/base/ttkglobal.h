#ifndef TTKGLOBAL_H
#define TTKGLOBAL_H

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


#if TTK_STD_CXX >= 202602L // c++2c
#  define TTK_HAS_CXX11 1
#  define TTK_HAS_CXX14 1
#  define TTK_HAS_CXX17 1
#  define TTK_HAS_CXX20 1
#  define TTK_HAS_CXX23 1
#  define TTK_HAS_CXX26 1
#elif TTK_STD_CXX >= 202302L // c++2b
#  define TTK_HAS_CXX11 1
#  define TTK_HAS_CXX14 1
#  define TTK_HAS_CXX17 1
#  define TTK_HAS_CXX20 1
#  define TTK_HAS_CXX23 1
#  define TTK_HAS_CXX26 0
#elif TTK_STD_CXX >= 202002L // c++2a
#  define TTK_HAS_CXX11 1
#  define TTK_HAS_CXX14 1
#  define TTK_HAS_CXX17 1
#  define TTK_HAS_CXX20 1
#  define TTK_HAS_CXX23 0
#  define TTK_HAS_CXX26 0
#elif TTK_STD_CXX >= 201703L // c++1z
#  define TTK_HAS_CXX11 1
#  define TTK_HAS_CXX14 1
#  define TTK_HAS_CXX17 1
#  define TTK_HAS_CXX20 0
#  define TTK_HAS_CXX23 0
#  define TTK_HAS_CXX26 0
#elif TTK_STD_CXX >= 201402L // c++1y
#  define TTK_HAS_CXX11 1
#  define TTK_HAS_CXX14 1
#  define TTK_HAS_CXX17 0
#  define TTK_HAS_CXX20 0
#  define TTK_HAS_CXX23 0
#  define TTK_HAS_CXX26 0
#elif TTK_STD_CXX >= 201103L // c++1x
#  define TTK_HAS_CXX11 1
#  define TTK_HAS_CXX14 0
#  define TTK_HAS_CXX17 0
#  define TTK_HAS_CXX20 0
#  define TTK_HAS_CXX23 0
#  define TTK_HAS_CXX26 0
#else
#  define TTK_HAS_CXX11 0
#  define TTK_HAS_CXX14 0
#  define TTK_HAS_CXX17 0
#  define TTK_HAS_CXX20 0
#  define TTK_HAS_CXX23 0
#  define TTK_HAS_CXX26 0
#endif


// C style format
using TTKInt8   = signed char;                  /* 8 bit signed */
using TTKUInt8  = unsigned char;                /* 8 bit unsigned */
using TTKInt16  = short;                        /* 16 bit signed */
using TTKUInt16 = unsigned short;               /* 16 bit unsigned */
using TTKInt32  = int;                          /* 32 bit signed */
using TTKUInt32 = unsigned int;                 /* 32 bit unsigned */
using TTKInt64  = long long;                    /* 64 bit signed */
using TTKUInt64 = unsigned long long;           /* 64 bit unsigned */

using TTKReal   = double;                       /* real */
using TTKDouble = double;                       /* double */
using TTKFloat  = float;                        /* float */
using TTKBool   = bool;                         /* bool */

// C++ style format
using TTKString  = std::string;                 /* string */
using TTKWString = std::wstring;                /* wstring */


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
#define TTK_CAT(a, b) a ## b
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
// marco str quotes
#define TTK_STR_QUOTES(s)   ("\"" + s + "\"")


// marco preprocessor overload
#define TTK_PP_OVERLOAD(prefix, ...) TTK_PP_CAT(prefix, TTK_PP_VARIADIC_SIZE(__VA_ARGS__))
#define TTK_PP_CAT(a, b) TTK_CAT(a, b)
#define TTK_PP_EMPTY()
#ifdef _MSC_VER
#  define TTK_PP_VARIADIC_SIZE(...) TTK_PP_CAT(TTK_PP_VARIADIC_SIZE_I(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, ), )
#else
#  define TTK_PP_VARIADIC_SIZE(...) TTK_PP_VARIADIC_SIZE_I(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, )
#endif
#define TTK_PP_VARIADIC_SIZE_I(e0, e1, e2, e3, e4, e5, e6, e7, size, ...) size


#define TTK_DOT          "."
#define TTK_DOTDOT       ".."
#define TTK_SPACE        " "
#define TTK_SEPARATOR    "/"
#define TTK_WSEPARATOR   "\\"
#define TTK_LINEFEED     "\n"
#define TTK_WLINEFEED    "\r\n"
#define TTK_PARENT_DIR   TTK_STR_CAT(TTK_DOTDOT, TTK_SEPARATOR)

#define TTK_SPLITER      "*|||*"
#define TTK_DEFAULT_STR  "-"
#define TTK_NAN_STR      "NaN"
#define TTK_NULL_STR     "null"

#define URL_PREFIX             "://"
#define HTTP_PROTOCOL_PREFIX   "http"
#define HTTPS_PROTOCOL_PREFIX  "https"

#define HTTP_PROTOCOL          TTK_STR_CAT(HTTP_PROTOCOL_PREFIX, URL_PREFIX)
#define HTTPS_PROTOCOL         TTK_STR_CAT(HTTPS_PROTOCOL_PREFIX, URL_PREFIX)


#define SHL_FILE_SUFFIX  "sh"
#define EXE_FILE_SUFFIX  "exe"
#define COM_FILE_SUFFIX  "com"

#define SHL_FILE         TTK_STR_CAT(TTK_DOT, SHL_FILE_SUFFIX)
#define EXE_FILE         TTK_STR_CAT(TTK_DOT, EXE_FILE_SUFFIX)
#define COM_FILE         TTK_STR_CAT(TTK_DOT, COM_FILE_SUFFIX)


// ttk date time format define
#define TTK_TIME_INIT           "00:00"
#define TTK_HOUR_FORMAT         "hh"
#define TTK_SECOND_FORMAT       "mm"
#define TTK_TIMEM_FORMAT        "hh:mm"
#define TTK_TIMES_FORMAT        "hh:mm:ss"
#define TTK_TIMEZ_FORMAT        "hh:mm:ss:zzz"
#define TTK_YEAR_FORMAT         "yyyy"
#define TTK_MONTH_FORMAT        "MM"
#define TTK_DAY_FORMAT          "dd"
#define TTK_WEEK_FORMAT         "dddd"
#define TTK_DATE_FORMAT         "yyyy-MM-dd"
#define TTK_DATE2_FORMAT        "yyyy.MM.dd"
#define TTK_DATE_TIMEM_FORMAT   "yyyy-MM-dd hh:mm"
#define TTK_DATE_TIMES_FORMAT   "yyyy-MM-dd hh:mm:ss"
#define TTK_DATE_TIMEZ_FORMAT   "yyyy-MM-dd hh:mm:ss:zzz"


// ttk item size define
#define TTK_ITEM_SIZE_3XS       5
#define TTK_ITEM_SIZE_2XS       10
#define TTK_ITEM_SIZE_XS        18
#define TTK_ITEM_SIZE_S         25
#define TTK_ITEM_SIZE_M         30
#define TTK_ITEM_SIZE_L         40
#define TTK_ITEM_SIZE_XL        50
#define TTK_ITEM_SIZE_2XL       60
#define TTK_ITEM_SIZE_3XL       75
#define TTK_ITEM_SIZE_4XL       90
#define TTK_ITEM_SIZE_5XL       105


// ttk level define
#define TTK_NONE_LEVEL          -3
#define TTK_LOW_LEVEL           -2
#define TTK_NORMAL_LEVEL        -1
#define TTK_HIGH_LEVEL          999


// ttk buffer define
#define TTK_LOW_BUFFER          256
#define TTK_NORMAL_BUFFER       512
#define TTK_HIGH_BUFFER         1024


// ttk date time number define
#define TTK_DN_NS           1

#define TTK_DN_US           1
#define TTK_DN_US2NS        1000

#define TTK_DN_MS           1
#define TTK_DN_MS2US        1000
#define TTK_DN_MS2NS        (TTK_DN_MS2US * TTK_DN_US2NS)
#define TTK_DN_ONCE         (50 * TTK_DN_MS)

#define TTK_DN_S            1
#define TTK_DN_S2MS         1000
#define TTK_DN_S2US         (TTK_DN_S2MS * TTK_DN_MS2US)
#define TTK_DN_S2NS         (TTK_DN_S2US * TTK_DN_US2NS)

#define TTK_DN_M            1
#define TTK_DN_M2S          60
#define TTK_DN_M2MS         (TTK_DN_M2S * TTK_DN_S2MS)
#define TTK_DN_M2US         TTKInt64(TTK_DN_M2MS * TTK_DN_MS2US)
#define TTK_DN_M2NS         TTKInt64(TTK_DN_M2US * TTK_DN_US2NS)

#define TTK_DN_H            1
#define TTK_DN_H2M          60
#define TTK_DN_H2S          (TTK_DN_H2M * TTK_DN_M2S)
#define TTK_DN_H2MS         (TTK_DN_H2S * TTK_DN_S2MS)
#define TTK_DN_H2US         TTKInt64(TTK_DN_H2MS * TTK_DN_MS2US)
#define TTK_DN_H2NS         TTKInt64(TTK_DN_H2US * TTK_DN_US2NS)

#define TTK_DN_D            1
#define TTK_DN_D2H          24
#define TTK_DN_D2M          (TTK_DN_D2H * TTK_DN_H2M)
#define TTK_DN_D2S          (TTK_DN_D2M * TTK_DN_M2S)
#define TTK_DN_D2MS         TTKInt64(TTK_DN_D2S * TTK_DN_S2MS)
#define TTK_DN_D2US         TTKInt64(TTK_DN_D2MS * TTK_DN_MS2US)
#define TTK_DN_D2NS         TTKInt64(TTK_DN_D2US * TTK_DN_US2NS)


// ttk size number define
#define TTK_SN_BT           1

#define TTK_SN_B            1
#define TTK_SN_B2BT         8

#define TTK_SN_KB           1
#define TTK_SN_KB2B         1024
#define TTK_SN_KB2BS        (TTK_SN_KB2B * TTK_SN_B2BT)

#define TTK_SN_MB           1
#define TTK_SN_MB2KB        1024
#define TTK_SN_MB2B         (TTK_SN_MB2KB * TTK_SN_KB2B)
#define TTK_SN_MB2BT        (TTK_SN_MB2B * TTK_SN_B2BT)

#define TTK_SN_GB           1
#define TTK_SN_GB2MB        1024
#define TTK_SN_GB2KB        (TTK_SN_GB2MB * TTK_SN_MB2KB)
#define TTK_SN_GB2B         TTKInt64(TTK_SN_GB2KB * TTK_SN_KB2B)
#define TTK_SN_GB2BT        TTKInt64(TTK_SN_GB2B * TTK_SN_B2BT)

#define TTK_SN_TB           1
#define TTK_SN_TB2GB        1024
#define TTK_SN_TB2MB        (TTK_SN_TB2GB * TTK_SN_GB2MB)
#define TTK_SN_TB2KB        TTKInt64(TTK_SN_TB2MB * TTK_SN_MB2KB)
#define TTK_SN_TB2B         TTKInt64(TTK_SN_TB2KB * TTK_SN_KB2B)
#define TTK_SN_TB2BT        TTKInt64(TTK_SN_TB2B * TTK_SN_B2BT)


// ttk angle number define
#define TTK_AN_0            0
#define TTK_AN_30           30
#define TTK_AN_45           45
#define TTK_AN_60           60
#define TTK_AN_90           90
#define TTK_AN_120          120
#define TTK_AN_180          180
#define TTK_AN_270          270
#define TTK_AN_360          360


// ttk bitrate number define
#define TTK_BN_0            0
#define TTK_BN_32           32
#define TTK_BN_64           64
#define TTK_BN_96           96
#define TTK_BN_128          128
#define TTK_BN_192          192
#define TTK_BN_250          250
#define TTK_BN_320          320
#define TTK_BN_500          500
#define TTK_BN_750          750
#define TTK_BN_1000         1000


// ttk range number define
#define TTK_RN_MIN          0
#define TTK_RN_MAX          100

#endif // TTKGLOBAL_H
