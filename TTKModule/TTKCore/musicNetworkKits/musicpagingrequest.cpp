#include "musicpagingrequest.h"

MusicPagingRequest::MusicPagingRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{
    m_pageSize = 0;
    m_pageTotal = 0;
    m_pageIndex = 0;

    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
#endif
}

MusicPagingRequest::~MusicPagingRequest()
{
    deleteAll();
}

void MusicPagingRequest::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}

void MusicPagingRequest::startToPage(int offset)
{
    Q_UNUSED(offset);
}
