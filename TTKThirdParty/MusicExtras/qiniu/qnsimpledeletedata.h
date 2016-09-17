#ifndef QNSIMPLEDELETEDATA_H
#define QNSIMPLEDELETEDATA_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QNetworkReply>
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
    explicit QNSimpleDeleteData(QNetworkAccessManager *networkManager,
                                QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    void deleteDataToServer(const QString &bucket, const QString &key);
    /*!
     * Delete data to server.
     */

Q_SIGNALS:
    void deleteFileFinished(bool state);
    /*!
     * Delete file finished.
     */

private Q_SLOTS:
    void receiveDataFromServer();
    /*!
     * Receive data from server.
     */
    void handleError(QNetworkReply::NetworkError error);
    /*!
     * Get handle error.
     */

private:
    TTK_DECLARE_PRIVATE(QNSimpleDeleteData)

};

#endif // QNSIMPLEDELETEDATA_H
