#ifndef QNSIMPLELISTDATA_H
#define QNSIMPLELISTDATA_H

#include <QNetworkReply>
#include "musicextrasglobaldefine.h"

class QNetworkAccessManager;

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
    QNetworkAccessManager *m_networkManager;

};

#endif // QNSIMPLELISTDATA_H
