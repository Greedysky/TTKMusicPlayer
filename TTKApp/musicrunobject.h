#ifndef MUSICRUNOBJECT_H
#define MUSICRUNOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QProcess>
#include <QDebug>
#include "musicobject.h"
#include "musicprivate.h"
#include "musicrunglobaldefine.h"

class MusicRunObjectPrivate;

/*! @brief The class of the music run object.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_RUN_EXPORT MusicRunObject : public QObject
{
    Q_OBJECT
public:
    explicit MusicRunObject(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    void checkValid();
    /*!
     * Check current setting file's validation.
     */
    void run(int argc, char **argv);
    /*!
     * To run main window.
     */
private Q_SLOTS:
    void finished(int code);
    /*!
     * Run finished.
     */

private:
    void dirIsExist(const QString &name);
    /*!
     * Check current dir is exist, no, just create it.
     */
    void checkTheDirectoryExist();
    /*!
     * Check the related dir is exist.
     */
    void checkTheFileNeededExist();
    /*!
     * Check the related file is exist.
     */

    TTK_DECLARE_PRIVATE(MusicRunObject)

};

#endif // MUSICRUNOBJECT_H
