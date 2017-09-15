#ifndef QNSIMPLEDELETEDATA_H
#define QNSIMPLEDELETEDATA_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QtNetwork/QNetworkReply>
#include "musicprivate.h"
#include "musicextrasglobaldefine.h"

class QNSimpleDeleteDataPrivate;

/*! @brief The namespace of the qiniu simple delete data.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNSimpleDeleteData : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit QNSimpleDeleteData(QNetworkAccessManager *networkManager,
                                QObject *parent = 0);
    /*!
     * Delete data to server.
     */
    void deleteDataToServer(const QString &bucket, const QString &key);

Q_SIGNALS:
    /*!
     * Delete file finished.
     */
    void deleteFileFinished(bool state);

private Q_SLOTS:
    /*!
     * Receive data from server.
     */
    void receiveDataFromServer();
    /*!
     * Get handle error.
     */
    void handleError(QNetworkReply::NetworkError error);

private:
    MUSIC_DECLARE_PRIVATE(QNSimpleDeleteData)

};

#endif // QNSIMPLEDELETEDATA_H
