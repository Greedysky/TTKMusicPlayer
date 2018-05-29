#include "musicdownloadpagingthread.h"

MusicDownLoadPagingThread::MusicDownLoadPagingThread(QObject *parent)
    : MusicNetworkAbstract(parent)
{
    m_pageSize = 0;
    m_pageTotal = 0;
    m_pageIndex = 0;

    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
#endif
}

MusicDownLoadPagingThread::~MusicDownLoadPagingThread()
{
    deleteAll();
}

void MusicDownLoadPagingThread::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}

void MusicDownLoadPagingThread::startToPage(int offset)
{
    Q_UNUSED(offset);
}
