#include "musicdownloadmanager.h"
#include "musicconnectionpool.h"
#include "musicdownloadstatusmodule.h"
#include "musicdownloadrecordwidget.h"
#include "musiccloudtablewidget.h"

void MusicDownLoadManager::connectNetworkMultiValue(QObject *object)
{
    m_queueList << object;
    const QObject *to = G_CONNECTION_PTR->value(MusicDownloadStatusModule::className());
    if(to)
    {
        connect(object, SIGNAL(downLoadDataChanged(QString)), to, SLOT(showDownLoadInfoFinished(QString)));
    }
}

void MusicDownLoadManager::removeNetworkMultiValue(QObject *object)
{
    const int index = m_queueList.indexOf(object);
    if(index != -1)
    {
        m_queueList.takeAt(index);
    }
}

void MusicDownLoadManager::connectDownload(const MusicDownLoadPairData &pair)
{
    QString className;
    switch(pair.m_type)
    {
        case TTK::Record::NormalDownload: className = MusicDownloadRecordTableWidget::className(); break;
        case TTK::Record::CloudDownload:  className = MusicCloudDownloadTableWidget::className(); break;
        case TTK::Record::CloudUpload:    className = MusicCloudUploadTableWidget::className(); break;
        default: break;
    }

    const QObject *to = G_CONNECTION_PTR->value(className);
    if(to && pair.m_object)
    {
        connect(pair.m_object, SIGNAL(createDownloadItem(QString, qint64)), to, SLOT(createDownloadItem(QString, qint64)));
        connect(pair.m_object, SIGNAL(downloadProgressChanged(float, QString, qint64)), to, SLOT(downloadProgressChanged(float, QString, qint64)));
    }

    connect(pair.m_object, SIGNAL(downloadProgressChanged(float, QString, qint64)), SLOT(downloadProgressChanged(float, QString, qint64)));
    m_pairList << pair;
}

void MusicDownLoadManager::reconnectDownload(const MusicDownLoadPairData &pair)
{
    const int index = m_pairList.indexOf(pair);
    if(index != -1)
    {
        MusicDownLoadPairData *p = &m_pairList[index];
        disconnect(p->m_object, SIGNAL(createDownloadItem(QString, qint64)), pair.m_object, SLOT(createDownloadItem(QString, qint64)));
        disconnect(p->m_object, SIGNAL(downloadProgressChanged(float, QString, qint64)), pair.m_object, SLOT(downloadProgressChanged(float, QString, qint64)));

        connect(p->m_object, SIGNAL(createDownloadItem(QString, qint64)), pair.m_object, SLOT(createDownloadItem(QString, qint64)));
        connect(p->m_object, SIGNAL(downloadProgressChanged(float, QString, qint64)), pair.m_object, SLOT(downloadProgressChanged(float, QString, qint64)));
    }
}

void MusicDownLoadManager::removeDownload(const MusicDownLoadPairData &pair)
{
    const int index = m_pairList.indexOf(pair);
    if(index != -1)
    {
        m_pairList.takeAt(index);
    }
}

void MusicDownLoadManager::downloadProgressChanged(float percent, const QString &total, qint64 time)
{
    Q_UNUSED(total);
    if(percent >= 100)
    {
        removeDownload(MusicDownLoadPairData(time));
    }
}
