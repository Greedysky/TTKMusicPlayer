#include "musicdjradiothreadabstract.h"

MusicDJRadioThreadAbstract::MusicDJRadioThreadAbstract(QObject *parent)
    : MusicNetworkAbstract(parent)
{
    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                       SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
#endif
}

QString MusicDJRadioThreadAbstract::getClassName()
{
    return staticMetaObject.className();
}

void MusicDJRadioThreadAbstract::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}
