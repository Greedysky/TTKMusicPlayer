#ifndef QNSIMPLEUPLOADDATA_H
#define QNSIMPLEUPLOADDATA_H

#include <QNetworkReply>

class QNetworkAccessManager;

class QNSimpleUploadData : public QObject
{
    Q_OBJECT
public:
    explicit QNSimpleUploadData(QNetworkAccessManager *networkManager,
                                QObject *parent = 0);

    void uploadDataToServer(const QByteArray &data, const QString &bucket,
                            const QString &key, const QString &name);

signals:
private slots:
    void receiveDataFromServer();
    void handleError(QNetworkReply::NetworkError error);

private:
    QNetworkAccessManager *m_networkManager;

};

#endif // QNSIMPLEUPLOADDATA_H
