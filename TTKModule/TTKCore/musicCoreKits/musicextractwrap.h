#ifndef MUSICEXTRACTWRAP_H
#define MUSICEXTRACTWRAP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicglobaldefine.h"

class MusicBackgroundImage;

/*! @brief The class of the extract data wrap.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicExtractWrap
{
public:
    static QString getClassName();
    /*!
     * Get class object name.
     */

    static bool outputThunderSkin(QPixmap &image, const QString &path);
    /*!
     * Transfer file to image data.
     */
    static bool outputSkin(MusicBackgroundImage *image, const QString &path);
    /*!
     * Transfer file to image data.
     */
    static bool inputSkin(MusicBackgroundImage *image, const QString &path);
    /*!
     * Transfer image data to file.
     */

};

#endif // MUSICEXTRACTWRAP_H
