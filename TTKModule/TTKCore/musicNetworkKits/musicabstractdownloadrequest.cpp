#include "musicabstractdownloadrequest.h"
#include "musicsettingmanager.h"
#include "musicdownloadmanager.h"
#include "musicstringutils.h"
#include "musiccoreutils.h"

#include <QSslError>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

MusicAbstractDownLoadRequest::MusicAbstractDownLoadRequest(const QString &url, const QString &save, MusicObject::DownloadType type, QObject *parent)
    : MusicAbstractNetwork(parent)
{
    m_url = url;
    m_savePath = save;
    m_downloadType = type;
    m_hasReceived = 0;
    m_currentReceived = 0;

    if(QFile::exists(m_savePath))
    {
        QFile::remove(m_savePath);
    }
    m_file = new QFile(m_savePath, this);

    M_DOWNLOAD_MANAGER_PTR->connectNetworkMultiValue(this);

    m_speedTimer.setInterval(MT_S2MS);
    connect(&m_speedTimer, SIGNAL(timeout()), SLOT(updateDownloadSpeed()));
}

MusicAbstractDownLoadRequest::~MusicAbstractDownLoadRequest()
{
    if(m_speedTimer.isActive())
    {
        m_speedTimer.stop();
    }
    M_DOWNLOAD_MANAGER_PTR->removeNetworkMultiValue(this);
}

void MusicAbstractDownLoadRequest::deleteAll()
{
    MusicAbstractNetwork::deleteAll();
    if(m_file)
    {
        delete m_file;
        m_file = nullptr;
    }
    deleteLater();
}

void MusicAbstractDownLoadRequest::replyError(QNetworkReply::NetworkError)
{
    TTK_LOGGER_ERROR("Abnormal network connection");
    Q_EMIT downLoadDataChanged("The file create failed");
    deleteAll();
}

#ifndef QT_NO_SSL
void MusicAbstractDownLoadRequest::sslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    sslErrorsString(reply, errors);
    Q_EMIT downLoadDataChanged("The file create failed");
    deleteAll();
}
#endif

void MusicAbstractDownLoadRequest::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    m_currentReceived = bytesReceived;
    m_totalSize = bytesTotal;
}

void MusicAbstractDownLoadRequest::updateDownloadSpeed()
{
    int delta = m_currentReceived - m_hasReceived;
    ///limit speed
    if(M_SETTING_PTR->value(MusicSettingManager::DownloadLimit).toInt() == 0)
    {
        const int limitValue = M_SETTING_PTR->value(MusicSettingManager::DownloadDLoadLimit).toInt();
        if(limitValue != 0 && delta > limitValue * MH_KB)
        {
            MusicUtils::Core::sleep(MT_S2MS - limitValue * MH_KB * MT_S2MS / delta);
            delta = limitValue * MH_KB;
        }
    }
    m_hasReceived = m_currentReceived;
}

QString MusicAbstractDownLoadRequest::mapCurrentQueryData() const
{
    switch(m_downloadType)
    {
        case MusicObject::DownloadMusic: return "DownloadMusic";
        case MusicObject::DownloadLrc:   return "DownloadLrc";
        case MusicObject::DownloadSmallBackground: return "DownloadSmallBackground";
        case MusicObject::DownloadBigBackground: return "DownloadBigBackground";
        case MusicObject::DownloadVideo: return "DownloadVideo";
        case MusicObject::DownloadOther: return "DownloadOther";
        default: return QString();
    }
}
