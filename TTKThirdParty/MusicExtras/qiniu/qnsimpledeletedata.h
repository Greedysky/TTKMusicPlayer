#ifndef QNSIMPLEDELETEDATA_H
#define QNSIMPLEDELETEDATA_H

#include <QNetworkReply>
#include "musicextrasglobaldefine.h"

class MUSIC_EXTRAS_EXPORT QNSimpleDeleteData : public QObject
{
    Q_OBJECT
public:
    explicit QNSimpleDeleteData(QNetworkAccessManager *networkManager,
                                QObject *parent = 0);
    void deleteDataToServer(const QString &bucket, const QString &key);

Q_SIGNALS:
    void deleteFileFinished(bool state);

private Q_SLOTS:
    void receiveDataFromServer();
    void handleError(QNetworkReply::NetworkError error);

private:
    QNetworkAccessManager *m_networkManager;

};

#endif // QNSIMPLEDELETEDATA_H
