#ifndef TTKQTGLOBAL_H
#define TTKQTGLOBAL_H

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
#include "ttkglobal.h"
#include "ttklogger.h"

#ifdef QT_DEBUG
#  define TTK_DEBUG
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


// declare list and enum flag
#define TTK_DECLARE_LIST(Class)        using Class##List = QList<Class>
#define TTK_DECLARE_FLAG(Flags, Enum)  using Flags = QFlags<Enum>

#if TTK_QT_VERSION_CHECK(6,0,0)
#  define qint qintptr
#else
#  define qint long
#endif

#define TTK_FILE_SUFFIX(fin)  fin.suffix().toLower()


// Qt style format
using TTKIntSet =           QSet<int>;                  /* int set */
using TTKIntList =          QList<int>;                 /* int list */
using TTKVariantList =      QList<QVariant>;            /* variant list */
using TTKStringMap =        QMap<QString, QString>;     /* strings map */
using TTKVariantMap =       QMap<QString, QVariant>;    /* string variant map */
using TTKIntStringMap =     QMap<qint64, QString>;      /* int string map */

#endif // TTKQTGLOBAL_H
