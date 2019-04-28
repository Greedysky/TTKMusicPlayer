#ifndef TTKGLOBAL_H
#define TTKGLOBAL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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


#ifdef Q_CC_GNU
#  pragma GCC diagnostic ignored "-Wunused-function"
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#  define TTK_GREATER_NEW
#  if QT_VERSION >= QT_VERSION_CHECK(5,2,0)
#    define TTK_WINEXTRAS
#  else
#    define TTK_NO_WINEXTRAS
#  endif
#else
#  define TTK_NO_WINEXTRAS
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
#  define MConst_cast(x,y) (const_cast< x >(y))
#else
#  define MConst_cast(x,y) ((x)(y))
#endif

#ifdef TTK_CAST
#  define MDynamic_cast(x,y) (dynamic_cast< x >(y))
#else
#  define MDynamic_cast(x,y) ((x)(y))
#endif

#ifdef TTK_CAST
#  define MReinterpret_cast(x,y) (reinterpret_cast< x >(y))
#else
#  define MReinterpret_cast(x,y) ((x)(y))
#endif

#ifdef TTK_CAST
#  define MStatic_cast(x,y) (static_cast< x >(y))
#else
#  define MStatic_cast(x,y) ((x)(y))
#endif

#ifdef TTK_CAST
#  define MObject_cast(x,y) (qobject_cast< x >(y))
#else
#  define MObject_cast(x,y) (qobject_cast< x >(y))
#endif

///
#define M_IN const &
#define M_OT &
#define M_RT const
#define M_RD &
#define M_RR const &
#define M_CT const
#define M_CR const &

/*
 * C++11 keywords and expressions
 */
#ifdef Q_COMPILER_NULLPTR
# define M_NULLPTR  nullptr
#else
# define M_NULLPTR  NULL
#endif

#ifdef Q_COMPILER_DEFAULT_MEMBERS
#  define M_DEFAULT = default
#else
#  define M_DEFAULT
#endif

#ifdef Q_COMPILER_DELETE_MEMBERS
# define M_DELETE = delete
#else
# define M_DELETE
#endif

#ifdef Q_COMPILER_EXPLICIT_OVERRIDES
# define M_OVERRIDE override
# define M_FINAL final
#else
# ifndef M_OVERRIDE
#  define M_OVERRIDE
# endif
# ifndef M_FINAL
#  define M_FINAL
# endif
#endif

///
#define TTK_ERROR_STR               "NaN"
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
    #define STRCAT(a, b)    a##b
  #else
    #define STRCAT(a, b)    a b
  #endif
#else
  #define STRCAT(a, b)      a b
#endif

#define TTK_DECLARE_LISTS(Class)            \
    typedef QList<Class> Class##s;

#define TTK_DECLARE_FLAGS(Flags, Enum)      \
    typedef QFlags<Enum> Flags;

///
typedef signed char                        MInt8;         /* 8 bit signed */
typedef unsigned char                      MUInt8;        /* 8 bit unsigned */
typedef short                              MInt16;        /* 16 bit signed */
typedef unsigned short                     MUInt16;       /* 16 bit unsigned */
typedef int                                MInt32;        /* 32 bit signed */
typedef unsigned int                       MUInt32;       /* 32 bit unsigned */
typedef long long                          MInt64;        /* 64 bit signed */
typedef unsigned long long                 MUInt64;       /* 64 bit unsigned */

typedef double                             MDouble;       /* double */
typedef float                              MFloat;        /* float */
typedef bool                               MBool;         /* bool */

typedef std::string                        MString;       /* string */

///
typedef QList<QVariant>                    MVariantList;            /* variantlist */
typedef QList<int>                         MIntList;                /* intlist */
typedef QSet<QVariant>                     MVariantSet;             /* variantset */
typedef QSet<int>                          MIntSet;                 /* intset */
typedef QMap<QString, QVariant>            MVariantMap;             /* stringVariantMap */
typedef QMap<QString, QString>             MStringMap;              /* stringsMap */
typedef QMap<QString, QStringList>         MStringListMap;          /* stringStrlistsMap */
typedef QMap<int, MIntList>                MIntListMap;             /* intIntlistMap */
typedef QMap<qint64, QString>              MIntStringMap;           /* intStrMap */
typedef QMapIterator<QString, QVariant>    MVariantMapIterator;     /* stringVariantMapIterator */
typedef QMapIterator<QString, QString>     MStringMapterator;       /* stringsMapIterator */
typedef QMapIterator<QString, QStringList> MStringListMapIterator;  /* stringStrlistsMapIterator */
typedef QMapIterator<int, MIntList>        MIntListMapIterator;     /* intIntlistMapIterator */
typedef QMapIterator<qint64, QString>      MIntStringMapIterator;   /* intStrMapIterator */

#endif // TTKGLOBAL_H
