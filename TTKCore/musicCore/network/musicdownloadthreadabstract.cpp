#include "musicdownloadthreadabstract.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QThread>
#if defined Q_OS_UNIX || defined Q_CC_MINGW
# include <unistd.h>
#endif
#include <QSslError>

MusicDownLoadThreadAbstract::MusicDownLoadThreadAbstract(const QString &url,
                const QString &save, Download_Type type, QObject *parent)
    : MusicNetworkAbstract(parent)
{
    m_url = url;
    m_savePathName = save;
    m_downloadType = type;
    m_hasReceived = -1;
    m_currentReceived = -1;

    if(QFile::exists(save))
    {
        QFile::remove(save);
    }
    m_file = new QFile(save, this);

    M_CONNECTION_PTR->setNetworkMultiValue(this);
    m_timer.setInterval(MT_S2MS);
    connect(&m_timer, SIGNAL(timeout()), SLOT(updateDownloadSpeed()));
}

MusicDownLoadThreadAbstract::~MusicDownLoadThreadAbstract()
{
    M_CONNECTION_PTR->removeNetworkMultiValue(this);
}

QString MusicDownLoadThreadAbstract::getClassName()
{
    return staticMetaObject.className();
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
    Q_UNUSED(bytesTotal);
    m_currentReceived = bytesReceived;
}

void MusicDownLoadThreadAbstract::updateDownloadSpeed()
{
    int delta = m_currentReceived - m_hasReceived;
    //////////////////////////////////////
    ///limit speed
    if(M_SETTING_PTR->value(MusicSettingManager::DownloadLimitChoiced).toInt() == 0)
    {
        int limitValue = M_SETTING_PTR->value(MusicSettingManager::DownloadDLoadLimitChoiced).toInt();
        if(limitValue != 0 && delta > limitValue*MH_KB)
        {
#if defined Q_OS_WIN && defined MUSIC_GREATER_NEW
            QThread::msleep(MT_S2MS - limitValue*MH_KB*MT_S2MS/delta);
#else
            usleep( (MT_S2MS - limitValue*MH_KB*MT_S2MS/delta)*MT_S2MS );
#endif
            delta = limitValue*MH_KB;
        }
    }
    //////////////////////////////////////
    m_hasReceived = m_currentReceived;
}
