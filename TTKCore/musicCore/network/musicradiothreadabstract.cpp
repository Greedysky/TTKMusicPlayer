#include "musicradiothreadabstract.h"

MusicRadioThreadAbstract::MusicRadioThreadAbstract(QObject *parent, QNetworkCookieJar *cookie)
    : QObject(parent), m_reply(nullptr), m_manager(nullptr)
{
    m_cookJar = cookie;
}

MusicRadioThreadAbstract::~MusicRadioThreadAbstract()
{
    deleteAll();
}

void MusicRadioThreadAbstract::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();;
        m_reply = nullptr;
    }
    if(m_manager)
    {
        m_manager->deleteLater();;
        m_manager = nullptr;
    }
}

void MusicRadioThreadAbstract::replyError(QNetworkReply::NetworkError)
{
    emit networkReplyFinished("The file create failed");
    deleteAll();
}
