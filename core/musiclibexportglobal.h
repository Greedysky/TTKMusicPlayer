#ifndef MUSICLIBEXPORTGLOBAL_H
#define MUSICLIBEXPORTGLOBAL_H

#include <QtCore/qglobal.h>

#ifdef QT_DEBUG
    #define MUSIC_DEBUG
#endif

#ifdef MUSIC_LIBRARY
#  define MUSIC_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_EXPORT Q_DECL_IMPORT
#endif

#endif // MUSICLIBEXPORTGLOBAL_H
