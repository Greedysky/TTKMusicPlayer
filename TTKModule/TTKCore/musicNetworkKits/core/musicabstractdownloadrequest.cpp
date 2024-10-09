#include "musicabstractdownloadrequest.h"
#include "musicdownloadmanager.h"
#include "musiccoreutils.h"

MusicAbstractDownLoadRequest::MusicAbstractDownLoadRequest(const QString &url, const QString &path, TTK::Download type, QObject *parent)
    : MusicAbstractNetwork(parent),
      m_url(url),
      m_savePath(path),
      m_downloadType(type),
      m_hasReceived(0),
      m_currentReceived(0)
{
    if(QFile::exists(m_savePath))
    {
        QFile::remove(m_savePath);
    }
    m_file = new QFile(m_savePath, this);

    G_DOWNLOAD_MANAGER_PTR->connectMultiNetwork(this);

    m_speedTimer.setInterval(TTK_DN_S2MS);
    connect(&m_speedTimer, SIGNAL(timeout()), SLOT(updateDownloadSpeed()));
}

MusicAbstractDownLoadRequest::~MusicAbstractDownLoadRequest()
{
    if(m_speedTimer.isActive())
    {
        m_speedTimer.stop();
    }
    G_DOWNLOAD_MANAGER_PTR->removeMultiNetwork(this);
}

void MusicAbstractDownLoadRequest::deleteAll()
{
    MusicAbstractNetwork::deleteAll();
    delete m_file;
    m_file = nullptr;
    deleteLater();
}

void MusicAbstractDownLoadRequest::downLoadFinished()
{
    MusicAbstractNetwork::downLoadFinished();
    m_speedTimer.stop();
}

void MusicAbstractDownLoadRequest::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    m_currentReceived = bytesReceived;
    m_totalSize = bytesTotal;
}

void MusicAbstractDownLoadRequest::updateDownloadSpeed()
{
    const int delta = m_currentReceived - m_hasReceived;
    ///limit speed
    if(G_SETTING_PTR->value(MusicSettingManager::DownloadLimitEnable).toInt() == 0)
    {
        const int limitValue = G_SETTING_PTR->value(MusicSettingManager::DownloadDownloadLimitSize).toInt();
        if(limitValue != 0 && delta > limitValue * TTK_SN_KB2B)
        {
            TTK::Core::sleep(TTK_DN_S2MS - limitValue * TTK_SN_KB2B * TTK_DN_S2MS / delta);
        }
    }
    m_hasReceived = m_currentReceived;
}

QString MusicAbstractDownLoadRequest::mapCurrentQueryData() const
{
    switch(m_downloadType)
    {
        case TTK::Download::Music: return DOWNLOAD_KEY_MUSIC;
        case TTK::Download::Lrc: return DOWNLOAD_KEY_LRC;
        case TTK::Download::Cover: return DOWNLOAD_KEY_COVER;
        case TTK::Download::Background: return DOWNLOAD_KEY_BACKGROUND;
        case TTK::Download::Video: return DOWNLOAD_KEY_VIDEO;
        case TTK::Download::Other: return DOWNLOAD_KEY_OTHER;
        default: return {};
    }
}
