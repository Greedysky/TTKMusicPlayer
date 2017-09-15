#ifndef QNSIMPLELISTDATA_H
#define QNSIMPLELISTDATA_H

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

class QNSimpleListDataPrivate;

typedef struct MUSIC_EXTRAS_EXPORT QNDataItem
{
    QString m_name;
    QString m_hash;
    QString m_mimeType;
    int m_size;
    int m_putTime;
}QNDataItem;
MUSIC_DECLARE_LISTS(QNDataItem)

/*! @brief The namespace of the qiniu simple list data.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNSimpleListData : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit QNSimpleListData(QNetworkAccessManager *networkManager,
                              QObject *parent = 0);
    /*!
     * List data to server.
     */
    void listDataToServer(const QString &bucket);

Q_SIGNALS:
    /*!
     * Receive data finshed.
     */
    void receiveFinshed(const QNDataItems &items);

private Q_SLOTS:
    /*!
     * Receive data from server finshed.
     */
    void receiveDataFromServer();
    /*!
     * Get handle error.
     */
    void handleError(QNetworkReply::NetworkError error);

private:
    MUSIC_DECLARE_PRIVATE(QNSimpleListData)

};

#endif // QNSIMPLELISTDATA_H
