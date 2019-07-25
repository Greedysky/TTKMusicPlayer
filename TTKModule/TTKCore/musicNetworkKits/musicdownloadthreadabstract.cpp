#include "musicdownloadthreadabstract.h"
#include "musicsettingmanager.h"
#include "musicdownloadmanager.h"
#include "musicstringutils.h"
#include "musiccoreutils.h"

#include <QSslError>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

MusicDownLoadThreadAbstract::MusicDownLoadThreadAbstract(const QString &url, const QString &save, MusicObject::DownloadType type, QObject *parent)
    : MusicNetworkAbstract(parent)
{
    m_url = url;
    m_savePathName = save;
    m_downloadType = type;
    m_hasReceived = 0;
    m_currentReceived = 0;

    if(QFile::exists(m_savePathName))
    {
        QFile::remove(m_savePathName);
    }
    m_file = new QFile(m_savePathName, this);

    M_DOWNLOAD_MANAGER_PTR->connectNetworkMultiValue(this);
    m_timer.setInterval(MT_S2MS);
    connect(&m_timer, SIGNAL(timeout()), SLOT(updateDownloadSpeed()));
}

MusicDownLoadThreadAbstract::~MusicDownLoadThreadAbstract()
{
    M_DOWNLOAD_MANAGER_PTR->removeNetworkMultiValue(this);
}

void MusicDownLoadThreadAbstract::deleteAll()
{
    MusicNetworkAbstract::deleteAll();
    if(m_file)
    {
        delete m_file;
        m_file = nullptr;
    }
    deleteLater();
}

void MusicDownLoadThreadAbstract::replyError(QNetworkReply::NetworkError)
{
    M_LOGGER_ERROR("Abnormal network connection");
    emit downLoadDataChanged("The file create failed");
    deleteAll();
}

#ifndef QT_NO_SSL
void MusicDownLoadThreadAbstract::sslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    sslErrorsString(reply, errors);
    emit downLoadDataChanged("The file create failed");
    deleteAll();
}
#endif

void MusicDownLoadThreadAbstract::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    m_currentReceived = bytesReceived;
    m_totalSize = bytesTotal;
}

void MusicDownLoadThreadAbstract::updateDownloadSpeed()
{
    int delta = m_currentReceived - m_hasReceived;
    ///limit speed
    if(M_SETTING_PTR->value(MusicSettingManager::DownloadLimitChoiced).toInt() == 0)
    {
        const int limitValue = M_SETTING_PTR->value(MusicSettingManager::DownloadDLoadLimitChoiced).toInt();
        if(limitValue != 0 && delta > limitValue*MH_KB)
        {
            MusicUtils::Core::sleep(MT_S2MS - limitValue*MH_KB*MT_S2MS/delta);
            delta = limitValue*MH_KB;
        }
    }
    m_hasReceived = m_currentReceived;
}

QString MusicDownLoadThreadAbstract::transferData() const
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
