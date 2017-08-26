#ifndef QIMAGEWRAP_H
#define QIMAGEWRAP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicextrasglobaldefine.h"

/*! @brief The namespace of the image wrapper.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT QImageWrap
{
public:
    QImage GaussFilter(const QImage &image, int radius = 10);
    /*!
     * Image gauss filter.
     */

};

#endif // QIMAGEWRAP_H
