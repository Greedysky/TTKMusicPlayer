#include "musictranslationrequest.h"

MusicTranslationRequest::MusicTranslationRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{
    m_reply = nullptr;
    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
#endif
}

void MusicTranslationRequest::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}
