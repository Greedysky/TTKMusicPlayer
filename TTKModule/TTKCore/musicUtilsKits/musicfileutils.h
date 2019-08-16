#ifndef MUSICFILEUTILS_H
#define MUSICFILEUTILS_H

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

#include "musicobject.h"
#include "musicnumberdefine.h"
#include "musicglobaldefine.h"

/*! @brief The namespace of the utils file.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUtils
{
    namespace File
    {
        /*!
         * Get given dir size.
         */
        MUSIC_UTILS_EXPORT quint64 dirSize(const QString &dirName);
        /*!
         * Check given dir size is bigger than given size.
         */
        MUSIC_UTILS_EXPORT void checkCacheSize(quint64 cacheSize, bool disabled, const QString &path);
        /*!
         * Get all files in given dir.
         */
        MUSIC_UTILS_EXPORT QFileInfoList getFileListByDir(const QString &dpath, bool recursively);
        /*!
         * Get all files in given dir.
         */
        MUSIC_UTILS_EXPORT QFileInfoList getFileListByDir(const QString &dpath, const QStringList &filter, bool recursively);
        /*!
         * Dir remove recursively.
         */
        MUSIC_UTILS_EXPORT bool removeRecursively(const QString &dir);

        /*!
         * Get open file dialog.
         */
        MUSIC_UTILS_EXPORT QString getOpenFileDialog(QWidget *obj, const QString &title, const QString &filter);
        /*!
         * Get open file dialog.
         */
        MUSIC_UTILS_EXPORT QString getOpenFileDialog(QWidget *obj, const QString &filter);
        /*!
         * Get open file dialog.
         */
        MUSIC_UTILS_EXPORT QString getOpenFileDialog(QWidget *obj);

        /*!
         * Get open files dialog.
         */
        MUSIC_UTILS_EXPORT QStringList getOpenFilesDialog(QWidget *obj, const QString &title, const QString &filter);
        /*!
         * Get open files dialog.
         */
        MUSIC_UTILS_EXPORT QStringList getOpenFilesDialog(QWidget *obj, const QString &filter);
        /*!
         * Get open files dialog.
         */
        MUSIC_UTILS_EXPORT QStringList getOpenFilesDialog(QWidget *obj);

        /*!
         * Get save file dialog.
         */
        MUSIC_UTILS_EXPORT QString getSaveFileDialog(QWidget *obj, const QString &title, const QString &filter);
        /*!
         * Get save file dialog.
         */
        MUSIC_UTILS_EXPORT QString getSaveFileDialog(QWidget *obj, const QString &filter);
        /*!
         * Get save file dialog.
         */
        MUSIC_UTILS_EXPORT QString getSaveFileDialog(QWidget *obj);

    }
}

#endif // MUSICFILEUTILS_H
