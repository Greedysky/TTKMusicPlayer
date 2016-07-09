#ifndef QNSIMPLEUPLOADDATA_H
#define QNSIMPLEUPLOADDATA_H

#include <QNetworkReply>
#include "musicextrasglobaldefine.h"

class QNetworkAccessManager;

class MUSIC_EXTRAS_EXPORT QNSimpleUploadData : public QObject
{
    Q_OBJECT
public:
    explicit QNSimpleUploadData(QNetworkAccessManager *networkManager,
                                QObject *parent = 0);

    void uploadDataToServer(const QByteArray &data, const QString &bucket,
                            const QString &key, const QString &name);

Q_SIGNALS:
    void uploadFileFinished(const QString &name);

private Q_SLOTS:
    void receiveDataFromServer();
    void handleError(QNetworkReply::NetworkError error);

private:
    QNetworkAccessManager *m_networkManager;

};

#endif // QNSIMPLEUPLOADDATA_H
