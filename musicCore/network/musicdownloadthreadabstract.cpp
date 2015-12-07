#include "musicdownloadthreadabstract.h"
#include "musicconnectionpool.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>

MusicDownLoadThreadAbstract::MusicDownLoadThreadAbstract(const QString &url,
                const QString &save, Download_Type type, QObject *parent)
    : QObject(parent), m_manager(nullptr), m_reply(nullptr)
{
    m_url = url;
    m_savePathName = save;
    m_downloadType = type;

    if(QFile::exists(save))
    {
        QFile::remove(save);
    }
    m_file = new QFile(save, this);

    M_CONNECTION->setNetworkMultiValue(this);
}

MusicDownLoadThreadAbstract::~MusicDownLoadThreadAbstract()
{
    M_CONNECTION->removeNetworkMultiValue(this);
}

void MusicDownLoadThreadAbstract::deleteAll()
{
    if(m_file)
    {
        delete m_file;
        m_file = nullptr;
    }
    if(m_manager)
    {
        m_manager->deleteLater();;
        m_manager = nullptr;
    }
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
    deleteLater();
}

void MusicDownLoadThreadAbstract::replyError(QNetworkReply::NetworkError)
{
    emit musicDownLoadFinished("The file create failed");
    deleteAll();
}
