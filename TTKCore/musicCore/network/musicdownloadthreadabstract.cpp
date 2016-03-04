#include "musicdownloadthreadabstract.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QThread>
#if defined Q_OS_UNIX || defined Q_CC_MINGW
# include <unistd.h>
#endif

MusicDownLoadThreadAbstract::MusicDownLoadThreadAbstract(const QString &url,
                const QString &save, Download_Type type, QObject *parent)
    : QObject(parent), m_manager(nullptr), m_reply(nullptr)
{
    m_url = url;
    m_savePathName = save;
    m_downloadType = type;
    m_hasRecevied = -1;
    m_currentRecevied = -1;

    if(QFile::exists(save))
    {
        QFile::remove(save);
    }
    m_file = new QFile(save, this);

    M_CONNECTION->setNetworkMultiValue(this);
    m_timer.setInterval(1000);
    connect(&m_timer, SIGNAL(timeout()), SLOT(updateDownloadSpeed()));
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

void MusicDownLoadThreadAbstract::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_UNUSED(bytesTotal);
    m_currentRecevied = bytesReceived;
}

void MusicDownLoadThreadAbstract::updateDownloadSpeed()
{
    int delta = m_currentRecevied - m_hasRecevied;
    //////////////////////////////////////
    ///limit speed
    if(M_SETTING->value(MusicSettingManager::DownloadLimitChoiced).toInt() == 0)
    {
        int limitValue = M_SETTING->value(MusicSettingManager::DownloadDLoadLimitChoiced).toInt();
        if(limitValue != 0 && delta > limitValue*1024)
        {
#if defined Q_OS_WIN && defined MUSIC_QT_5
            QThread::msleep(1000 - limitValue*1024*1000/delta);
#else
            usleep( (1000 - limitValue*1024*1000/delta)*1000 );
#endif
            delta = limitValue*1024;
        }
    }
    //////////////////////////////////////
    m_hasRecevied = m_currentRecevied;
}
