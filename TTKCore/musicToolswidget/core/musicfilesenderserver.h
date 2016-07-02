#ifndef MUSICFILESENDERSERVER_H
#define MUSICFILESENDERSERVER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QFile>
#include <QUdpSocket>
#include <QStringList>
#include "musicglobaldefine.h"

#define MAX_DATA        8000
#define SEND_PORT       7754
#define RECEVIE_PORT    7755
#define SPLITE_STR      "*|||*"
#define DATAGRAM        "BACK_ON"

/*! @brief The class of the file sender server.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicFileSenderServer : public QObject
{
    Q_OBJECT
public:
    explicit MusicFileSenderServer(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicFileSenderServer();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void start();
    /*!
     * Start to transfer file.
     */
    void setSendFile(const QString &file);
    /*!
     * Set send file.
     */
    void setSendFiles(const QStringList &files);
    /*!
     * Set send file list.
     */
    void setReceiveAddress(const QString &ip);
    /*!
     * Set receive address.
     */
    QString getReceiveAddress() const;
    /*!
     * Get receive address.
     */

Q_SIGNALS:
    void fileSendFinished();
    /*!
     * Send file just has finished.
     */

private Q_SLOTS:
    void readPendingDatagrams();
    /*!
     * Read pending datagrams.
     */

protected:
    void sendData();
    /*!
     * Send file data.
     */

    bool m_nextFile;
    QUdpSocket *m_sendSocket;
    QFile *m_file;
    QStringList m_files;
    QString m_receiveIp;

};

#endif // MUSICFILESENDERSERVER_H
