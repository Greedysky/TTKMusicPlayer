#ifndef MUSICSERVICEOBJECT_H
#define MUSICSERVICEOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QProcess>
#include "musicserviceglobaldefine.h"

/*! @brief The class of the music service object.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SERVICE_EXPORT MusicServiceObject : public QObject
{
    Q_OBJECT
public:
    explicit MusicServiceObject(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicServiceObject();

    void run();
    /*!
     * To run main window.
     */

protected:
    QProcess *m_process;

};

#endif // MUSICSERVICEOBJECT_H
