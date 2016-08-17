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
#include "musicprivate.h"
#include "musicserviceglobaldefine.h"

class MusicServiceObjectPrivate;

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

    void run();
    /*!
     * To run main window.
     */

private:
    TTK_DECLARE_PRIVATE(MusicServiceObject)

};

#endif // MUSICSERVICEOBJECT_H
