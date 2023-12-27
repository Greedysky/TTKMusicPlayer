#ifndef MUSICFILEUTILS_H
#define MUSICFILEUTILS_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include "musicurlutils.h"

/*! @brief The namespace of the utils file.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    namespace File
    {   
        /*!
         * Get all files in given dir.
         */
        TTK_MODULE_EXPORT QStringList fileListByPath(const QString &dpath, const QStringList &filter = {}, bool recursively = true);
        /*!
         * Get all files infos in given dir.
         */
        TTK_MODULE_EXPORT QFileInfoList fileInfoListByPath(const QString &dpath, const QStringList &filter = {}, bool recursively = true);
        /*!
         * Copy path from srcPath to dstPath.
         */
        TTK_MODULE_EXPORT bool copyPath(const QString &srcPath, const QString &dstPath, bool overwrite);
        /*!
         * Dir remove recursively.
         */
        TTK_MODULE_EXPORT bool removeRecursively(const QString &dir, bool self = true);

        /*!
         * Get open existing directory dialog.
         */
        TTK_MODULE_EXPORT QString getExistingDirectory(QWidget *parent);
        /*!
         * Get open file name dialog.
         */
        TTK_MODULE_EXPORT QString getOpenFileName(QWidget *parent, const QString &filter = "Image Files (*.png *.bmp *.jpg)");
        /*!
         * Get open file names dialog.
         */
        TTK_MODULE_EXPORT QStringList getOpenFileNames(QWidget *parent, const QString &filter = "Image Files (*.png *.bmp *.jpg)");
        /*!
         * Get save file name dialog.
         */
        TTK_MODULE_EXPORT QString getSaveFileName(QWidget *parent, const QString &filter = "Image Files (*.png *.bmp *.jpg)");

    }
}

#endif // MUSICFILEUTILS_H
