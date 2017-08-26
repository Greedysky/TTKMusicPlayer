#ifndef QNCONF_H
#define QNCONF_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicextrasglobaldefine.h"

/*! @brief The namespace of the qiniu config.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNConf
{
public:
    static QString UPLOAD_HOST;
    static QString RS_HOST;
    static QString RSF_HOST;
    static QString IOVIP_HOST;

    static QString ACCESS_KEY;
    static QByteArray SECRET_KEY;

    static QString DEFAULT_STYLE_DELIMITER;

    static QString VERSION;

};

#endif // QNCONF_H
