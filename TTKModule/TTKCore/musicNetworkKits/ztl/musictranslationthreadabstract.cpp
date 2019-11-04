#include "musictranslationthreadabstract.h"

MusicTranslationThreadAbstract::MusicTranslationThreadAbstract(QObject *parent)
    : MusicNetworkAbstract(parent)
{
    m_reply = nullptr;
    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
#endif
}

void MusicTranslationThreadAbstract::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}
