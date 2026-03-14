#include "musicabstractnetwork.h"

MusicAbstractNetwork::MusicAbstractNetwork(QObject *parent)
    : TTKAbstractNetwork(parent)
{

}

void MusicAbstractNetwork::replyError(QNetworkReply::NetworkError code)
{
    TTK_ERROR_STREAM("Abnormal network connection, module" << this << "code" << code);
    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
    if(reply)
    {
        TTK_ERROR_STREAM("Abnormal network url is" << reply->request().url());
    }

//    Q_EMIT downloadDataChanged({});
    deleteAll();
}

#ifndef QT_NO_SSL
void MusicAbstractNetwork::sslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    sslErrorsString(reply, errors);
//    Q_EMIT downloadDataChanged({});
    deleteAll();
}
#endif
