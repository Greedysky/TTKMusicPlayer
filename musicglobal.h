#ifndef MUSICGLOBAL_H
#define MUSICGLOBAL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMap>
#include <QSet>
#include <QVariant>
#include <QtCore/qglobal.h>

#ifdef Q_CC_GNU
#  pragma GCC diagnostic ignored "-Wunused-function"
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#  define MUSIC_GREATER_NEW
#  if QT_VERSION >= QT_VERSION_CHECK(5,2,0)
#    define MUSIC_WINEXTRAS
#  else
#    define MUSIC_NO_WINEXTRAS
#    if QT_VERSION >= QT_VERSION_CHECK(5,1,0)
#      define MUSIC_GREATER_FIVE_ONE
#    else
#      define MUSIC_GREATER_FIVE_ZERO
#    endif
#  endif
#endif

#ifdef QT_DEBUG
#   define MUSIC_DEBUG
#endif

//////////////////////////////////////
#ifdef __cplusplus
#  define MUSIC_CAST
#endif
//////////////////////////////////////

//////////////////////////////////////
///cast
///
#ifdef MUSIC_CAST
#  define MConst_cast(x,y) (const_cast< x >(y))
#else
#  define MConst_cast(x,y) ((x)(y))
#endif

#ifdef MUSIC_CAST
#  define MDynamic_cast(x,y) (dynamic_cast< x >(y))
#else
#  define MDynamic_cast(x,y) ((x)(y))
#endif

#ifdef MUSIC_CAST
#  define MReinterpret_cast(x,y) (reinterpret_cast< x >(y))
#else
#  define MReinterpret_cast(x,y) ((x)(y))
#endif

#ifdef MUSIC_CAST
#  define MStatic_cast(x,y) (static_cast< x >(y))
#else
#  define MStatic_cast(x,y) ((x)(y))
#endif

#ifdef MUSIC_CAST
#  define MObject_cast(x,y) (qobject_cast< x >(y))
#else
#  define MObject_cast(x,y) (qobject_cast< x >(y))
#endif

#define TTK_DECLARE_LISTS(Class) typedef QList<Class> Class##s;

//////////////////////////////////////
/*! @brief The namespace of the application object.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicObject
{
    typedef signed char                        MInt8;         /* 8 bit signed */
    typedef unsigned char                      MUint8;        /* 8 bit unsigned */
    typedef short                              MInt16;        /* 16 bit signed */
    typedef unsigned short                     MUint16;       /* 16 bit unsigned */
    typedef int                                MInt32;        /* 32 bit signed */
    typedef unsigned int                       MUint32;       /* 32 bit unsigned */
    typedef long long                          MInt64;        /* 64 bit signed */
    typedef unsigned long long                 MUint64;       /* 64 bit unsigned */
    ///////////////////////////////////////

    typedef double                             MDouble;       /* double */
    typedef float                              MFloat;        /* float */
    typedef bool                               MBool;         /* bool */
    ///////////////////////////////////////

    typedef QList<QVariant>                    MVariantList;            /* variantlist */
    typedef QList<QStringList>                 MStringLists;            /* stringlists */
    typedef QList<int>                         MIntList;                /* intlist */
    typedef QList<MIntList>                    MIntLists;               /* intlists */
    typedef QSet<QVariant>                     MVariantSet;             /* variantset */
    typedef QSet<int>                          MIntSet;                 /* intset */
    typedef QSet<MIntSet>                      MIntSets;                /* intsets */
    typedef QMap<QVariant, QVariant>           MVariantsMap;            /* variantsmap */
    typedef QMap<QString, QVariant>            MStriantMap;             /* stringVariantMap */
    typedef QMap<QString, QString>             MStringsMap;             /* stringsMap */
    typedef QMap<QString, QStringList>         MStringsListMap;         /* stringStrlistsMap */
    typedef QMap<int, MIntList>                MIntsListMap;            /* intIntlistMap */
    typedef QMap<qint64, QString>              MIntStringMap;           /* intStrMap */
    typedef QMapIterator<QVariant, QVariant>   MVariantsMapIterator;    /* variantsmMapIterator */
    typedef QMapIterator<QString, QVariant>    MStriantMapIterator;     /* stringVariantMapIterator */
    typedef QMapIterator<QString, QString>     MStringsMapIterator;     /* stringsMapIterator */
    typedef QMapIterator<QString, QStringList> MStringsListMapIterator; /* stringStrlistsMapIterator */
    typedef QMapIterator<int, MIntList>        MIntsListMapIterator;    /* intIntlistMapIterator */
    typedef QMapIterator<qint64, QString>      MIntStringMapIterator;   /* intStrMapIterator */
    ///////////////////////////////////////
}


#endif // MUSICGLOBAL_H
