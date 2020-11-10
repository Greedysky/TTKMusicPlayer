#ifndef TTKGLOBAL_H
#define TTKGLOBAL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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
 ================================================= */

#include <QMap>
#include <QSet>
#include <QVariant>
#include <QtCore/qglobal.h>
#include "ttklogger.h"

#ifdef Q_CC_GNU
#  pragma GCC diagnostic ignored "-Wunused-function"
#endif

#define TTK_QT_VERSION_CHECK(major, minor, patch) (QT_VERSION >= QT_VERSION_CHECK(major, minor, patch))
#if TTK_QT_VERSION_CHECK(5,0,0)
#  define TTK_GREATER_NEW
#  if TTK_QT_VERSION_CHECK(5,2,0)
#    define TTK_WINEXTRAS
#  else
#    define TTK_NO_WINEXTRAS
#  endif
#else
#  define TTK_NO_WINEXTRAS
#endif

#if !TTK_QT_VERSION_CHECK(5,7,0)
#define TTK_AS_CONST
// this adds const to non-const objects (like std::as_const)
template <typename T>
Q_DECL_CONSTEXPR typename std::add_const<T>::type &qAsConst(T &t) noexcept { return t; }
// prevent rvalue arguments:
template <typename T>
void qAsConst(const T &&) = delete;
#endif

//
#ifdef QT_DEBUG
#   define TTK_DEBUG
#endif

///
#ifdef __cplusplus
#  define TTK_CAST
#endif

///cast
#ifdef TTK_CAST
#  define TTKConst_cast(x,y) (const_cast< x >(y))
#else
#  define TTKConst_cast(x,y) ((x)(y))
#endif

#ifdef TTK_CAST
#  define TTKDynamic_cast(x,y) (dynamic_cast< x >(y))
#else
#  define TTKDynamic_cast(x,y) ((x)(y))
#endif

#ifdef TTK_CAST
#  define TTKReinterpret_cast(x,y) (reinterpret_cast< x >(y))
#else
#  define TTKReinterpret_cast(x,y) ((x)(y))
#endif

#ifdef TTK_CAST
#  define TTKStatic_cast(x,y) (static_cast< x >(y))
#else
#  define TTKStatic_cast(x,y) ((x)(y))
#endif

#ifdef TTK_CAST
#  define TTKObject_cast(x,y) (qobject_cast< x >(y))
#else
#  define TTKObject_cast(x,y) (qobject_cast< x >(y))
#endif

///
#define TTK_IN const &
#define TTK_OT &
#define TTK_RT const
#define TTK_RD &
#define TTK_RR const &
#define TTK_CT const
#define TTK_CR const &

/*
 * C++11 keywords and expressions
 */
#ifdef Q_COMPILER_NULLPTR
# define TTK_NULLPTR  nullptr
#else
# define TTK_NULLPTR  NULL
#endif

#ifdef Q_COMPILER_DEFAULT_MEMBERS
#  define TTK_DEFAULT = default
#else
#  define TTK_DEFAULT
#endif

#ifdef Q_COMPILER_DELETE_MEMBERS
# define TTK_DELETE = delete
#else
# define TTK_DELETE
#endif

#ifdef Q_COMPILER_EXPLICIT_OVERRIDES
# define TTK_OVERRIDE override
# define TTK_FINAL final
#else
# ifndef TTK_OVERRIDE
#  define TTK_OVERRIDE
# endif
# ifndef TTK_FINAL
#  define TTK_FINAL
# endif
#endif

///
#define TTK_NAN_STR                 "NaN"
#define TTK_STR_SPLITER             "*|||*"

///
#define TTK_DECLARE_PTR(Class)              \
public:                                     \
    using Class##Ptr = Class *;             \
    using Class##CPtr = const Class *;

#define TTK_DECLARE_GCN(Class)              \
public:                                     \
    inline static QString getClassName()    \
    {                                       \
        return #Class;                      \
    }

#define TTK_DECLARE_MODULE(Class)           \
public:                                     \
    TTK_DECLARE_PTR(Class)                  \
    TTK_DECLARE_GCN(Class)

///
#ifndef _MSC_VER
  //gcc version less than 3.4.0
  #if __GNUC__ <= 3 && __GNUC_MINOR__ <= 4
    #define TTK_STRCAT(a, b)    a##b
  #else
    #define TTK_STRCAT(a, b)    a b
  #endif
#else
  #define TTK_STRCAT(a, b)      a b
#endif

#define TTK_DECLARE_LISTS(Class)            \
    typedef QList<Class> Class##s;

#define TTK_DECLARE_FLAGS(Flags, Enum)      \
    typedef QFlags<Enum> Flags;

///
typedef signed char                        TTKInt8;         /* 8 bit signed */
typedef unsigned char                      TTKUInt8;        /* 8 bit unsigned */
typedef short                              TTKInt16;        /* 16 bit signed */
typedef unsigned short                     TTKUInt16;       /* 16 bit unsigned */
typedef int                                TTKInt32;        /* 32 bit signed */
typedef unsigned int                       TTKUInt32;       /* 32 bit unsigned */
typedef long long                          TTKInt64;        /* 64 bit signed */
typedef unsigned long long                 TTKUInt64;       /* 64 bit unsigned */

typedef double                             TTKDouble;       /* double */
typedef float                              TTKFloat;        /* float */
typedef bool                               TTKBool;         /* bool */

typedef std::string                        TTKString;       /* string */

///
typedef QList<QVariant>                    TTKVariantList;            /* variantlist */
typedef QList<int>                         TTKIntList;                /* intlist */
typedef QSet<QVariant>                     TTKVariantSet;             /* variantset */
typedef QSet<int>                          TTKIntSet;                 /* intset */
typedef QMap<QString, QVariant>            TTKVariantMap;             /* stringVariantMap */
typedef QMap<QString, QString>             TTKStringMap;              /* stringsMap */
typedef QMap<QString, QStringList>         TTKStringListMap;          /* stringStrlistsMap */
typedef QMap<int, TTKIntList>              TTKIntListMap;             /* intIntlistMap */
typedef QMap<qint64, QString>              TTKIntStringMap;           /* intStrMap */
typedef QMapIterator<QString, QVariant>    TTKVariantMapIterator;     /* stringVariantMapIterator */
typedef QMapIterator<QString, QString>     TTKStringMapterator;       /* stringsMapIterator */
typedef QMapIterator<QString, QStringList> TTKStringListMapIterator;  /* stringStrlistsMapIterator */
typedef QMapIterator<int, TTKIntList>      TTKIntListMapIterator;     /* intIntlistMapIterator */
typedef QMapIterator<qint64, QString>      TTKIntStringMapIterator;   /* intStrMapIterator */

#endif // TTKGLOBAL_H
