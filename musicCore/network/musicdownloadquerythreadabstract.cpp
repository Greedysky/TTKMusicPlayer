#include "musicdownloadquerythreadabstract.h"
#include "musicconnectionpool.h"

MusicDownLoadQueryThreadAbstract::MusicDownLoadQueryThreadAbstract(QObject *parent)
    : QObject(parent), m_reply(nullptr)
{
    m_searchQuality = "标准品质";
    m_manager = new QNetworkAccessManager(this);
    m_queryAllRecords = false;
    M_CONNECTION->setValue("MusicDownLoadQueryThread", this);
    M_CONNECTION->poolConnect("MusicDownLoadQueryThread", "MusicDownloadStatusLabel");
}

MusicDownLoadQueryThreadAbstract::~MusicDownLoadQueryThreadAbstract()
{
    M_CONNECTION->poolDisConnect("MusicDownLoadQueryThread");
    deleteAll();///The release of all the objects
    if(m_manager)
    {
        m_manager->deleteLater();
        m_manager = nullptr;
    }
}

void MusicDownLoadQueryThreadAbstract::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}

void MusicDownLoadQueryThreadAbstract::replyError(QNetworkReply::NetworkError)
{
    M_LOGGER << "Abnormal network connection" << LOG_END;
    deleteAll();
}
