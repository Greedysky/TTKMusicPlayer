#ifndef MUSICEXTRACTMANAGER_H
#define MUSICEXTRACTMANAGER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include "musicglobaldefine.h"

struct MusicBackgroundImage;

/*! @brief The class of the extract data manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicExtractManager
{
    TTK_DECLARE_MODULE(MusicExtractManager)
public:
    /*!
     * Transfer file to image data.
     */
    static bool outputThunderSkin(QPixmap &image, const QString &input);

    /*!
     * Transfer file to normal data.
     */
    static bool outputBinary(const QString &input, const QString &output, QStringList &path);

    /*!
     * Transfer file to image data.
     */
    static bool outputSkin(MusicBackgroundImage *image, const QString &input);
    /*!
     * Transfer image data to file.
     */
    static bool inputSkin(MusicBackgroundImage *image, const QString &output);

    /*!
     * Transfer file to data.
     */
    static bool outputData(QByteArray &data, const QString &input);
    /*!
     * Transfer data to file.
     */
    static bool inputData(const QByteArray &data, const QString &output);

};

#endif // MUSICEXTRACTMANAGER_H
