#ifndef MUSICFILERECEIVESERVER_H
#define MUSICFILERECEIVESERVER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicfilesenderserver.h"

/*! @brief The class of the file receive server.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SUPER_EXPORT MusicFileReceiveServer : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFileReceiveServer(QObject *parent = 0);

    ~MusicFileReceiveServer();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set save file dir.
     */
    void setSavePathDir(const QString &dir);

private Q_SLOTS:
    /*!
     * Read pending datagrams.
     */
    void readPendingDatagrams();

protected:
    QFile *m_file;
    QUdpSocket *m_receiveSocket;
    QString m_saveDir;

};

#endif // MUSICFILERECEIVESERVER_H
