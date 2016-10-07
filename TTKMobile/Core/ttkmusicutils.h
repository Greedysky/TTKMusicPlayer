#ifndef TTKMUSICUTILS_H
#define TTKMUSICUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicmobileglobaldefine.h"

/*! @brief The class of the music utils.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKMusicUtils : public QObject
{
    Q_OBJECT
public:
    explicit TTKMusicUtils(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    Q_INVOKABLE QString getRoot() const;
    /*!
     * Get root.
     */
    Q_INVOKABLE QString getRootPath() const;
    /*!
     * Get root path.
     */
    Q_INVOKABLE QString normalizeTime(qint64 time, const QString &format);
    /*!
     * Transfer integer time to string by format.
     */

};

#endif // TTKMUSICUTILS_H
