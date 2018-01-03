#ifndef MUSICCOREUTILS_H
#define MUSICCOREUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

#include "musicobject.h"
#include "musicnumberdefine.h"
#include "musicglobaldefine.h"

/*! @brief The class of the utils core object namespace.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUtils
{
    namespace Core
    {
        /*!
         * Get lrc dir prefix path.
         */
        MUSIC_UTILS_EXPORT QString lrcPrefix();
        /*!
         * Get music dir prefix path.
         */
        MUSIC_UTILS_EXPORT QString musicPrefix();
        /*!
         * Get file suffix path.
         */
        MUSIC_UTILS_EXPORT QString fileSuffix(const QString &name);

        /*!
         * Get given dir size.
         */
        MUSIC_UTILS_EXPORT quint64 dirSize(const QString &dirName);
        /*!
         * Check given dir size is bigger than given size.
         */
        MUSIC_UTILS_EXPORT void checkCacheSize(quint64 cacheSize, bool disabled, const QString &path);
        /*!
         * To find file by dir path.
         */
        MUSIC_UTILS_EXPORT QFileInfoList findFile(const QString &path, const QStringList &filter);

        /*!
         * Get Language Name by given index.
         */
        MUSIC_UTILS_EXPORT QString getLanguageName(int index);

        /*!
         * Dir remove recursively.
         */
        MUSIC_UTILS_EXPORT bool removeRecursively(const QString &dir);

        /*!
         * Open file from local by path or net url.
         */
        MUSIC_UTILS_EXPORT bool openUrl(const QString &exe, const QString &path);

        /*!
         * Open file from local by path or net url.
         */
        MUSIC_UTILS_EXPORT bool openUrl(const QString &path, bool local = true);

        /*!
         * Set string to unicode string by format.
         */
        MUSIC_UTILS_EXPORT QString toUnicode(const char *chars, const char *format = "GBK");
        /*!
         * Set string to unicode string by format.
         */
        MUSIC_UTILS_EXPORT QString toUnicode(const QByteArray &chars, const char *format = "GBK");
        /*!
         * Set string from unicode string by format.
         */
        MUSIC_UTILS_EXPORT QByteArray fromUnicode(const QString &chars, const char *format = "GBK");
        /*!
         * Set local codec by format.
         */
        MUSIC_UTILS_EXPORT void setLocalCodec(const char *format = "utf-8");
        /*!
         * Trasform string to local 8bit char.
         */
        MUSIC_UTILS_EXPORT const char *toLocal8Bit(const QString &str);
        /*!
         * Trasform string to utf8 char.
         */
        MUSIC_UTILS_EXPORT const char *toUtf8(const QString &str);

        /*!
         * Get qmmp plugin dir path.
         */
        MUSIC_UTILS_EXPORT QString pluginPath(const QString &module, const QString &format);
        /*!
         * Mid config file transfer operator.
         */
        MUSIC_UTILS_EXPORT void midTransferFile();
        /*!
         * Music version check.
         */

        MUSIC_UTILS_EXPORT bool musicVersionCheck(const QStringList &ol, const QStringList &dl, int depth);
        /*!
         * Music version check.
         */
        MUSIC_UTILS_EXPORT bool musicVersionCheck(const QString &o, const QString &d);

    }
}

#endif // MUSICCOREUTILS_H
