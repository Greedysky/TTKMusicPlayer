#include "musicabstractnetwork.h"

MusicAbstractNetwork::MusicAbstractNetwork(QObject *parent)
    : TTKAbstractNetwork(parent)
{

}

void MusicAbstractNetwork::replyError(QNetworkReply::NetworkError)
{
    TTK_ERROR_STREAM("Abnormal network connection");
//    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

#ifndef QT_NO_SSL
void MusicAbstractNetwork::sslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    sslErrorsString(reply, errors);
//    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
#endif
