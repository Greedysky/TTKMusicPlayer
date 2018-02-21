#include "musicdownloadmanager.h"
#include "musicobject.h"
#include "musicconnectionpool.h"
#ifndef MUSIC_MOBILE
#include "musicdownloadstatusobject.h"
#include "musicdownloadrecordwidget.h"
#include "musiccloudtablewidget.h"
#endif

QString MusicDownLoadManager::getClassName()
{
    return "MusicDownLoadManager";
}

void MusicDownLoadManager::setNetworkMultiValue(QObject *object)
{
    m_queueList << object;
    QObject *to = M_CONNECTION_PTR->value( MusicDownloadStatusObject::getClassName() );
    if(to != nullptr)
    {
        QObject::connect(object, SIGNAL(downLoadDataChanged(QString)), to, SLOT(showDownLoadInfoFinished(QString)));
    }
}

void MusicDownLoadManager::removeNetworkMultiValue(QObject *object)
{
    int index = m_queueList.indexOf(object);
    if(index != -1)
    {
        m_queueList.takeAt(index);
    }
}

void MusicDownLoadManager::connectMusicDownload(QObject *object)
{
#ifndef MUSIC_MOBILE
    QObject *to = M_CONNECTION_PTR->value( MusicDownloadRecordWidget::getClassName() );
    if(to != nullptr && object)
    {
        QObject::connect(object, SIGNAL(downloadProgressChanged(float, QString, qint64)), to, SLOT(downloadProgressChanged(float, QString, qint64)));
        QObject::connect(object, SIGNAL(createDownloadItem(QString, qint64)), to, SLOT(createDownloadItem(QString, qint64)));
    }

    to = M_CONNECTION_PTR->value( MusicCloudDownloadTableWidget::getClassName() );
    if(to != nullptr && object)
    {
        QObject::connect(object, SIGNAL(downloadProgressChanged(float, QString, qint64)), to, SLOT(downloadProgressChanged(float, QString, qint64)));
        QObject::connect(object, SIGNAL(createDownloadItem(QString, qint64)), to, SLOT(createDownloadItem(QString, qint64)));
    }
#else
    Q_UNUSED(object);
#endif
}
