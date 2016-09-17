#ifndef QNSIMPLELISTDATA_H
#define QNSIMPLELISTDATA_H

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

class QNSimpleListDataPrivate;

typedef struct MUSIC_EXTRAS_EXPORT QNDataItem
{
    QString m_name;
    QString m_hash;
    QString m_mimeType;
    int m_size;
    int m_putTime;
}QNDataItem;
TTK_DECLARE_LISTS(QNDataItem)

/*! @brief The namespace of the qiniu simple list data.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNSimpleListData : public QObject
{
    Q_OBJECT
public:
    explicit QNSimpleListData(QNetworkAccessManager *networkManager,
                              QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    void listDataToServer(const QString &bucket);
    /*!
     * List data to server.
     */

Q_SIGNALS:
    void receiveFinshed(const QNDataItems &items);
    /*!
     * Receive data finshed.
     */

private Q_SLOTS:
    void receiveDataFromServer();
    /*!
     * Receive data from server finshed.
     */
    void handleError(QNetworkReply::NetworkError error);
    /*!
     * Get handle error.
     */

private:
    TTK_DECLARE_PRIVATE(QNSimpleListData)

};

#endif // QNSIMPLELISTDATA_H
