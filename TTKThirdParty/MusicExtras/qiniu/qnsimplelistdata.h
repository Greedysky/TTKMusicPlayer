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

typedef struct QNDataItem
{
    QString m_name;
    QString m_hash;
    QString m_mimeType;
    int m_size;
    int m_putTime;
}QNDataItem;
typedef QList<QNDataItem> QNDataItems;

class MUSIC_EXTRAS_EXPORT QNSimpleListData : public QObject
{
    Q_OBJECT
public:
    explicit QNSimpleListData(QNetworkAccessManager *networkManager,
                              QObject *parent = 0);
    void listDataToServer(const QString &bucket);

Q_SIGNALS:
    void receiveFinshed(const QNDataItems &items);

private Q_SLOTS:
    void receiveDataFromServer();
    void handleError(QNetworkReply::NetworkError error);

private:
    TTK_DECLARE_PRIVATE(QNSimpleListData)

};

#endif // QNSIMPLELISTDATA_H
