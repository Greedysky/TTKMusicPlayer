#ifndef TTKGLOBAL_H
#define TTKGLOBAL_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QMap>
#include <QSet>
#include <QVariant>
#include "ttklogger.h"

// Normal definition
#undef TTK_STD_CXX
#define TTK_STD_CXX __cplusplus

// VS2013 - 2015
#if defined Q_CC_MSVC && _MSC_VER >= 1800
#undef TTK_STD_CXX
#define TTK_STD_CXX 201103L
#endif

// VS2017
#if defined Q_CC_MSVC && _MSC_VER >= 1910
#undef TTK_STD_CXX
#define TTK_STD_CXX 201402L
#endif

// VS2019
#if defined Q_CC_MSVC && _MSC_VER >= 1920
#undef TTK_STD_CXX
#define TTK_STD_CXX 201703L
#endif

// VS2022
#if defined Q_CC_MSVC && _MSC_VER >= 1930
#undef TTK_STD_CXX
#define TTK_STD_CXX 202002L
#endif

#ifdef Q_CC_GNU
#  pragma GCC diagnostic ignored "-Wswitch"
#  pragma GCC diagnostic ignored "-Wparentheses"
#  pragma GCC diagnostic ignored "-Wunused-result"
#  pragma GCC diagnostic ignored "-Wunused-function"
#endif

#define TTK_QT_VERSION_CHECK(major, minor, patch) (QT_VERSION >= QT_VERSION_CHECK(major, minor, patch))

#ifndef qPrintable
#  define qPrintable(s) QString(s).toLocal8Bit().constData()
#endif

#ifndef qUtf8Printable
#  define qUtf8Printable(s) QString(s).toUtf8().constData()
#endif

#if !TTK_QT_VERSION_CHECK(5,7,0)
#  define TTK_AS_CONST
// this adds const to non-const objects (like std::as_const)
template <typename T>
Q_DECL_CONSTEXPR typename std::add_const<T>::type &qAsConst(T &t) noexcept { return t; }
// prevent rvalue arguments:
template <typename T>
void qAsConst(const T &&) = delete;
#endif

#ifdef QT_DEBUG
#  define TTK_DEBUG
#endif

#if TTK_STD_CXX >= 201103L
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

#ifdef TTK_CAST
#  define TTKObjectCast(x, y) (qobject_cast<x>(y))
#else
#  define TTKObjectCast(x, y) ((x)(y))
#endif

#if defined TTK_CAST && TTK_QT_VERSION_CHECK(5,15,0)
#  define TTKVoidCast(x) (x)(void*)
#else
#  define TTKVoidCast(x) (x)
#endif

// deprecated function
#ifdef Q_CC_MSVC
#  define TTK_DEPRECATED          __declspec(deprecated)
#  define TTK_DEPRECATED_X(text)  __declspec(deprecated(text))
#else
#  define TTK_DEPRECATED          __attribute__((__deprecated__))
#  define TTK_DEPRECATED_X(text)  __attribute__((__deprecated__(text)))
#endif

#if !TTK_QT_VERSION_CHECK(5,0,0) && defined(Q_CC_GNU)
#  if defined(__GXX_EXPERIMENTAL_CXX0X__) || TTK_STD_CXX >= 201103L
#    define Q_COMPILER_DEFAULT_MEMBERS
#    define Q_COMPILER_DELETE_MEMBERS
#    define Q_COMPILER_NULLPTR
#    define Q_COMPILER_EXPLICIT_OVERRIDES
#    define Q_COMPILER_CONSTEXPR
#  endif
#endif

// C++11 keywords and expressions
#ifdef Q_COMPILER_NULLPTR
#  define TTK_NULLPTR  nullptr
#else
#  define TTK_NULLPTR  NULL
#endif

#ifdef Q_COMPILER_DEFAULT_MEMBERS
#  define TTK_DEFAULT = default
#else
#  define TTK_DEFAULT
#endif

#ifdef Q_COMPILER_DELETE_MEMBERS
#  define TTK_DELETE = delete
#else
#  define TTK_DELETE
#endif

#ifdef Q_COMPILER_EXPLICIT_OVERRIDES
#  define TTK_OVERRIDE override
#  define TTK_FINAL final
#else
#  ifndef TTK_OVERRIDE
#    define TTK_OVERRIDE
#  endif
#  ifndef TTK_FINAL
#    define TTK_FINAL
#  endif
#endif

#if defined Q_CC_MSVC && _MSC_VER <= 1800
#  define constexpr const
#endif

#if defined Q_COMPILER_CONSTEXPR
#  if defined(__cpp_constexpr) && __cpp_constexpr >= 201304L
#    define TTK_CONSTEXPR constexpr
#    define TTK_RCONSTEXPR constexpr
#  else
#    define TTK_CONSTEXPR constexpr
#    define TTK_RCONSTEXPR const
#  endif
#else
#  define TTK_CONSTEXPR const
#  define TTK_RCONSTEXPR const
#endif

// disable copy
#define TTK_DISABLE_COPY(Class) \
private: \
  Class(const Class &) TTK_DELETE; \
  Class &operator=(const Class &) TTK_DELETE;

// disable init and copy
#define TTK_DISABLE_INIT_COPY(Class) \
  TTK_DISABLE_COPY(Class) \
  Class() TTK_DELETE;

// make class name
#define TTK_DECLARE_MODULE(Class) \
public: \
  inline static QString className() \
  { \
    return #Class; \
  }

// marco cat
#define TTK_CAT(a, b) a##b
// marco str cat
#ifndef Q_CC_MSVC
// gcc version less than 3.4.0
#  if __GNUC__ <= 3 && __GNUC_MINOR__ <= 4
#    define TTK_STRCAT(a, b) a##b
#  else
#    define TTK_STRCAT(a, b) a b
#  endif
#else
#  define TTK_STRCAT(a, b) a b
#endif

// marco preprocessor overload
#define TTK_PP_OVERLOAD(prefix, ...) TTK_PP_CAT(prefix, TTK_PP_VARIADIC_SIZE(__VA_ARGS__))
#define TTK_PP_CAT(a, b) TTK_CAT(a, b)
#define TTK_PP_EMPTY()
#ifdef Q_CC_MSVC
#  define TTK_PP_VARIADIC_SIZE(...) TTK_PP_CAT(TTK_PP_VARIADIC_SIZE_I(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1,),)
#else
#  define TTK_PP_VARIADIC_SIZE(...) TTK_PP_VARIADIC_SIZE_I(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1,)
#endif
#define TTK_PP_VARIADIC_SIZE_I(e0, e1, e2, e3, e4, e5, e6, e7, size, ...) size

// declare list and enum flag
#define TTK_DECLARE_LIST(Class)        using Class##List = QList<Class>
#define TTK_DECLARE_FLAG(Flags, Enum)  using Flags = QFlags<Enum>

#define TTK_DOT             "."
#define TTK_DOTDOT          ".."
#define TTK_SEPARATOR       "/"
#define TTK_PDIR            TTK_STRCAT(TTK_DOTDOT, TTK_SEPARATOR)

#define TTK_SPLITER         "*|||*"
#define TTK_DEFAULT_STR     "-"
#define TTK_NAN_STR         "NaN"
#define TTK_NULL_STR        "null"

#if TTK_QT_VERSION_CHECK(6,0,0)
#  define qint qintptr
#else
#  define qint long
#endif

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

// Qt style format
using TTKIntSet =           QSet<int>;                  /* int set */
using TTKIntList =          QList<int>;                 /* int list */
using TTKVariantList =      QList<QVariant>;            /* variant list */
using TTKStringMap =        QMap<QString, QString>;     /* strings map */
using TTKVariantMap =       QMap<QString, QVariant>;    /* string variant map */
using TTKIntStringMap =     QMap<qint64, QString>;      /* int string map */

#endif // TTKGLOBAL_H
