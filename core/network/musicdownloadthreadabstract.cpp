#include "musicdownloadthreadabstract.h"
#include "musicconnectionpool.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>

MusicDownLoadThreadAbstract::MusicDownLoadThreadAbstract(const QString &url,
                const QString &save, Download_Type type, QObject *parent)
    : QObject(parent), m_reply(NULL), m_manager(NULL)
{
    m_url = url;
    m_savePathName = save;
    m_downloadType = type;

    if(QFile::exists(save))
    {
        QFile::remove(save);
    }
    m_file = new QFile(save, this);

    M_Connection->setNetworkMultiValue(this);
}

MusicDownLoadThreadAbstract::~MusicDownLoadThreadAbstract()
{
    M_Connection->removeNetworkMultiValue(this);
}

void MusicDownLoadThreadAbstract::deleteAll()
{
    if(m_file)
    {
        delete m_file;
        m_file = NULL;
    }
    if(m_manager)
    {
        m_manager->deleteLater();;
        m_manager = NULL;
    }
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = NULL;
    }
    deleteLater();
}

void MusicDownLoadThreadAbstract::replyError(QNetworkReply::NetworkError)
{
    emit musicDownLoadFinished("The file create failed");
    deleteAll();
}
