#include "musicdownloaddiscoverlistthread.h"
#include "musictime.h"

MusicDownLoadDiscoverListThread::MusicDownLoadDiscoverListThread(QObject *parent)
    : MusicNetworkAbstract(parent)
{
    MusicTime::timeSRand();
    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                       SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
#endif
}

MusicDownLoadDiscoverListThread::~MusicDownLoadDiscoverListThread()
{
    deleteAll();
}

QString MusicDownLoadDiscoverListThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadDiscoverListThread::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}
