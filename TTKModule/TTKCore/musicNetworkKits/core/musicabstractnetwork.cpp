#include "musicabstractnetwork.h"

MusicAbstractNetwork::MusicAbstractNetwork(QObject *parent)
    : TTKAbstractNetwork(parent)
{

}

void MusicAbstractNetwork::replyError(QNetworkReply::NetworkError error)
{
    TTK_ERROR_STREAM("Abnormal network connection, module" << this << "code" << error);
//    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

#ifndef QT_NO_SSL
void MusicAbstractNetwork::sslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    sslErrorsString(reply, errors);
//    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
#endif
