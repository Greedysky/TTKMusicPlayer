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
